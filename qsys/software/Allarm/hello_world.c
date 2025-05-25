#include "system.h" // 系统头文件
#include "altera_avalon_timer_regs.h" // 定时器头文件
#include "altera_avalon_pio_regs.h" // PIO 头文件
#include "sys/alt_irq.h" // 中断头文件
#include "unistd.h" // usleep 延时函数头文件
#include "alt_types.h"
#include <stdio.h> // 标准输入输出头文件
#include <altera_avalon_lcd_16207_regs.h> // LCD 控制头文件
#include <math.h>  // [MATRIX] 矩阵运算需要

/**/
#define MAX_ITER 100       // 增加迭代次数
#define TOLERANCE 1e-7f    // 提高精度要求
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX_SIZE 20           // 最大支持矩阵尺寸（根据FPGA资源调整）

alt_u8 mul_flag;
alt_u8 dis_flag=0;
//矩阵结构体（静态内存）
typedef struct {
    int rows;
    int cols;
    float data[MAX_SIZE][MAX_SIZE];
} Matrix;

//矩阵加法
void matrix_add(const Matrix *A, const Matrix *B, Matrix *C)
{
	C->rows = MAX(A->rows,B->rows);
	C->cols = MAX(A->cols,B->cols);
	for (int i = 0; i < C->rows; i++)
		for (int j = 0; j < C->cols; j++)
			C->data[i][j]=0;
	for (int i = 0; i < A->rows; i++)
		for (int j = 0; j < A->cols; j++)
			C->data[i][j]=C->data[i][j]+A->data[i][j];
	for (int i = 0; i < B->rows; i++)
		for (int j = 0; j < B->cols; j++)
			C->data[i][j]=C->data[i][j]+B->data[i][j];
}

//矩阵减法
void matrix_sub(const Matrix *A, const Matrix *B, Matrix *C)
{
	C->rows = MAX(A->rows,B->rows);
	C->cols = MAX(A->cols,B->cols);
	for (int i = 0; i < C->rows; i++)
		for (int j = 0; j < C->cols; j++)
			C->data[i][j]=0;
	for (int i = 0; i < A->rows; i++)
		for (int j = 0; j < A->cols; j++)
			C->data[i][j]=C->data[i][j]+A->data[i][j];
	for (int i = 0; i < B->rows; i++)
		for (int j = 0; j < B->cols; j++)
			C->data[i][j]=C->data[i][j]-B->data[i][j];
}

//矩阵乘法
void matrix_multiply(const Matrix *A, const Matrix *B, Matrix *C)
{
    if (A->cols != B->rows) mul_flag=0;
    else mul_flag=1;

    if(mul_flag==1)
    {
		C->rows = A->rows;
		C->cols = B->cols;

		for (int i = 0; i < A->rows; i++)
		{
			for (int j = 0; j < B->cols; j++)
			{
				C->data[i][j] = 0;
				for (int k = 0; k < A->cols; k++)
				{
					C->data[i][j] += A->data[i][k] * B->data[k][j];
				}
			}
		}
    }
}

//矩阵求逆
int matrix_inverse(const Matrix *input, Matrix *output)
{
    if (input->rows != input->cols) return -1;
    int n = input->rows;
    float mat[MAX_SIZE][2*MAX_SIZE] = {0};

    // 构造增广矩阵 [I|A]
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[i][j] = input->data[i][j];
            mat[i][j + n] = (i == j) ? 1 : 0;
        }
    }

    // 高斯消元
    for (int pivot = 0; pivot < n; pivot++)
    {
        if (fabs(mat[pivot][pivot]) < 1e-6) return -2; // 奇异矩阵

        // 归一化主行
        float scale = mat[pivot][pivot];
        for (int j = pivot; j < 2*n; j++)
        {
            mat[pivot][j] /= scale;
        }

        // 消去其他行
        for (int i = 0; i < n; i++)
        {
            if (i != pivot) {
                float factor = mat[i][pivot];
                for (int j = pivot; j < 2*n; j++)
                {
                    mat[i][j] -= factor * mat[pivot][j];
                }
            }
        }
    }

    // 提取逆矩阵
    output->rows = output->cols = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            output->data[i][j] = mat[i][j + n];
        }
    }
    return 0;
}

void svd_decomposition(const Matrix *A, Matrix *U, Matrix *S, Matrix *V) {
    int m = A->rows, n = A->cols;
    const int size = MIN(m, n);

    // 初始化S矩阵为A的副本
    S->rows = m; S->cols = n;
    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            S->data[i][j] = A->data[i][j];

    // 初始化U和V为单位矩阵
    U->rows = U->cols = m;
    V->rows = V->cols = n;
    for(int i=0; i<MAX_SIZE; i++){
        for(int j=0; j<MAX_SIZE; j++){
            U->data[i][j] = (i==j && i<m) ? 1.0f : 0.0f;
            V->data[i][j] = (i==j && i<n) ? 1.0f : 0.0f;
        }
    }

    // 改进的雅可比迭代
    for(int iter=0; iter<MAX_ITER; iter++){
        float max_off = 0.0f;
        int p = 0, q = 0;

        // 寻找最大非对角元素（全矩阵扫描）
        for(int i=0; i<size; i++){
            for(int j=i+1; j<size; j++){
                float off_diag = fabs(S->data[i][j]) + fabs(S->data[j][i]);
                if(off_diag > max_off){
                    max_off = off_diag;
                    p = i; q = j;
                }
            }
        }
        if(max_off < TOLERANCE) break;

        // 计算2x2子矩阵
        float a = S->data[p][p], b = S->data[p][q];
        float c = S->data[q][p], d = S->data[q][q];

        // 计算精确旋转角度（双边雅可比）
        float theta = 0.5f * atan2(2*(b + c), a - d);
        float cos_theta = cosf(theta);
        float sin_theta = sinf(theta);

        // 左乘旋转矩阵（行操作）
        float temp_row[MAX_SIZE];
        for(int j=0; j<n; j++){
            temp_row[j] = S->data[p][j];
            S->data[p][j] =  cos_theta * temp_row[j] + sin_theta * S->data[q][j];
            S->data[q][j] = -sin_theta * temp_row[j] + cos_theta * S->data[q][j];
        }

        // 更新U矩阵
        for(int i=0; i<m; i++){
            float Uip = U->data[i][p];
            float Uiq = U->data[i][q];
            U->data[i][p] =  cos_theta * Uip + sin_theta * Uiq;
            U->data[i][q] = -sin_theta * Uip + cos_theta * Uiq;
        }

        // 右乘旋转矩阵（列操作）
        float temp_col[MAX_SIZE];
        for(int i=0; i<m; i++){
            temp_col[i] = S->data[i][p];
            S->data[i][p] =  cos_theta * temp_col[i] + sin_theta * S->data[i][q];
            S->data[i][q] = -sin_theta * temp_col[i] + cos_theta * S->data[i][q];
        }

        // 更新V矩阵
        for(int j=0; j<n; j++){
            float Vjp = V->data[j][p];
            float Vjq = V->data[j][q];
            V->data[j][p] =  cos_theta * Vjp + sin_theta * Vjq;
            V->data[j][q] = -sin_theta * Vjp + cos_theta * Vjq;
        }
    }

    // 确保奇异值非负
    for(int i=0; i<size; i++){
        if(S->data[i][i] < 0){
            S->data[i][i] = -S->data[i][i];
            for(int j=0; j<m; j++)
                U->data[j][i] = -U->data[j][i];
        }
    }

    // 奇异值排序（稳定冒泡排序）
    for(int i=0; i<size; i++){
        for(int j=i+1; j<size; j++){
            if(S->data[i][i] < S->data[j][j]){
                // 交换奇异值
                float s_val = S->data[i][i];
                S->data[i][i] = S->data[j][j];
                S->data[j][j] = s_val;

                // 交换U的列
                for(int k=0; k<m; k++){
                    float tmp = U->data[k][i];
                    U->data[k][i] = U->data[k][j];
                    U->data[k][j] = tmp;
                }

                // 交换V的列
                for(int k=0; k<n; k++){
                    float tmp = V->data[k][i];
                    V->data[k][i] = V->data[k][j];
                    V->data[k][j] = tmp;
                }
            }
        }
    }
}

void matrix_print(const Matrix *mat)
{
    for (int i = 0; i < mat->rows; i++)
    {
        for (int j = 0; j < mat->cols; j++)
        {
            printf("%8.3f ", mat->data[i][j]); // 通过JTAG UART输出
        }
        printf("\n");
    }
}

/**/
// Hex 到 7 段数码管转换表
unsigned char table[16] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F};

// 函数声明
void IRQ_Init(); // 中断初始化函数
void IRQ_Key_Interrupts1(); // 中断服务子程序（按钮1）
void IRQ_Key_Interrupts2(); // 中断服务子程序（按钮2）
void IRQ_Key_Interrupts3(); // 中断服务子程序（按钮3）
void DISP(int time); // 显示函数
void Timer_Initial(void); // 初始化定时器中断
void Timer_ISR(void* isr_context); // 定时器中断服务函数
void Timer_Start(void);
void Timer_Stop(void);
void check_busy(); // 检查 LCD 忙状态
void lcd_init(); // LCD 初始化
void delay(); // 延时函数
void lcd_prints(alt_u8 *string); 	// 打印字符串到 LCD
void select_xy(alt_u8 x, alt_u8 y); // 选择 LCD 光标位置

// 全局变量
alt_u32 timer_isr_context; // 定义全局变量用于存储 isr_context 指针
alt_u8 add_func = 0; // 默认为不设置，1——设置当前时间，2——设置闹钟时间
alt_u32 actual_time = 0, alarm_time = 5, alarm_active = 0, lcd_active = 0;
alt_u32 count = 0;
alt_u8 lcd_string1[] = "CYH&HSH CLOCK";
alt_u8 lcd_string2[] =        "TIME!            ";
alt_u8 lcd_alarm_time[9]={0};
alt_u8 lcd_flag = 0;
alt_u8 i1=0;
alt_u8 i2=0;
alt_u8 i3=0;
alt_u8 mode = 0;
alt_u8 matrix_done = 0;  // [MATRIX] 标记矩阵是否已执行

// 主函数
int main(void) {
    IRQ_Init(); // 初始化 PIO 中断
    lcd_init(); // LCD 初始化
    select_xy(0, 0);
    lcd_prints(lcd_string1);
    Timer_Initial(); // 初始化定时器中断
    IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x0); // 初始化 LEDR 全灭
	Matrix A, B, C, invA, U, S, V;
    while (1) {
    	 // 读取 sw3 决定模式
    	    if ((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE) & 0x8) != 0)
    	        mode = 1; // 矩阵运算模式
    	     else
    	        mode = 0; // 闹钟模式
    	    if (mode == 0)
    	    {
    	    	//时间到——LCD
					if(actual_time-alarm_time>5)
					{
						i1=1;
						lcd_active=0;
						i3=0;
					}
					else i1=i2=0;
					if(lcd_active==0&&i1==1&&i2==0)
					{
						IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x01);   //清屏
						usleep(5000);
						select_xy(0, 0);
						lcd_prints(lcd_string1);

						i2=1;
					}
					if(lcd_flag==1 && lcd_active==1)//如果时间到了，LCD显示TIME
					{
						IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x01);   //清屏
						usleep(5000);
						select_xy(0,0);
						lcd_prints(lcd_string2);
						lcd_flag=0;
						lcd_active=0;
					}
    	    	        // 时间到——红灯
    	    	        if (alarm_active != 0)
    	    	        {
    	    	            IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x3); // 如果时间到，红灯亮 5 秒
    	    	            lcd_flag = 1; // LCD 显示
    	    	        }
    	    	        else
    	    	           IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x0);
    	    	        // 设置 sliders 状态检测
					if (((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE)) & 1) != 0)
					{ // 闹钟开关打开
						IOWR_ALTERA_AVALON_PIO_DATA(LEDG_BASE, 0x1); // 绿灯开
						if (((actual_time - alarm_time) >= 0) && (actual_time - alarm_time) <= 5)
						{
							alarm_active = 1; // 红灯亮 5 秒
							if(i3==0)
							{
								lcd_active=1;
								i3=1;
							}
						}
						else
						{
							alarm_active = 0;
							lcd_active = 1;
						}
    	    	     }
					else
					{
						IOWR_ALTERA_AVALON_PIO_DATA(LEDG_BASE, 0x0); // 绿灯关
						alarm_active = 0;
    	    	    }

    	    	        // 设置当前时间
    	    	        if (((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE)) & 4) != 0) {
    	    	           // DISP(actual_time);
    	    	        	Timer_Stop();
    	    	            add_func = 1; // button1 中断允许
    	    	        }
    	    	        // 设置闹钟时间
    	    	        else if (((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE)) & 2) != 0) {
    	    	            DISP(alarm_time);
    	    	            //Timer_Stop();
    	    	            dis_flag=1;
    	    	            add_func = 2; // button2 中断允许
    	    	        } else {
    	    	            //DISP(actual_time);
    	    	        	Timer_Start();
    	    	            add_func = 0;
    	    	            dis_flag=0;
    	    	        }
    	       } else if (mode == 1 && matrix_done == 0) {
    	           matrix_done = 1;
    	           //输入矩阵A
    	           	    printf("\n请输入矩阵A的行数和列数: ");
    	           	    scanf("%d %d", &A.rows, &A.cols);
    	           	    printf("请按行输入矩阵A的 %d 个元素（用空格分隔）:\n", A.rows*A.cols);
    	           	    for(int i=0; i<A.rows; i++)
    	           	    {
    	           	        for(int j=0; j<A.cols; j++)
    	           	        {
    	           	            scanf("%f", &A.data[i][j]);
    	           	        }
    	           	    }
    	           	//输入矩阵B
    	           	    printf("\n请输入矩阵B的行数和列数: ");
    	           	    scanf("%d %d", &B.rows, &B.cols);
    	           	    printf("请按行输入矩阵B的 %d 个元素（用空格分隔）:\n", B.rows*B.cols);
    	           	    for(int i=0; i<B.rows; i++)
    	           	    {
    	           	        for(int j=0; j<B.cols; j++)
    	           	        {
    	           	           scanf("%f", &B.data[i][j]);
    	           	        }
    	           	    }
    	               //矩阵加法
    	           	    matrix_add(&A, &B, &C);
    	           	    printf("\n矩阵加法结果\n");
    	           	    matrix_print(&C);
    	           	//矩阵减法
    	           	    matrix_sub(&A, &B, &C);
    	           	    printf("\n矩阵减法结果\n");
    	           	    matrix_print(&C);
    	           	//矩阵乘法
    	           		matrix_multiply(&A, &B, &C);
    	           		printf("\n矩阵乘法结果:\n");
    	           		if(mul_flag==0) printf("\nError!\n");
    	           		if(mul_flag==1) matrix_print(&C);
    	           	//矩阵求逆
    	           		printf("\n矩阵求逆结果\n");
    	           		if (matrix_inverse(&A, &invA) == 0)	matrix_print(&invA);
    	           		else printf("\nError!\n");
    	           	//测试SVD分解
    	           		svd_decomposition(&A, &U, &S, &V);
    	           		printf("\n矩阵SVD分解结果：\n");
    	           		printf("\nU Matrix:\n");
    	           		matrix_print(&U);
    	           		printf("\nS Matrix:\n");
    	           		matrix_print(&S);
    	           		printf("\nV Matrix:\n");
    	           		matrix_print(&V);
    	       }
    }
}

void IRQ_Init() { // 初始化 PIO 中断
    // button1
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON1_BASE, 1); // 使能中断
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON1_BASE, 1); // 清中断边沿检测寄存器
    alt_ic_isr_register(
        BUTTON1_IRQ_INTERRUPT_CONTROLLER_ID, // 中断控制器 ID
        BUTTON1_IRQ, // 硬件中断号
        IRQ_Key_Interrupts1, // 中断服务函数
        0x0, // ISR 上下文指针
        0x0); // flags，保留未用

    // button2
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON2_BASE, 1); // 使能中断
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON2_BASE, 1); // 清中断边沿检测寄存器
    alt_ic_isr_register(
        BUTTON2_IRQ_INTERRUPT_CONTROLLER_ID,
        BUTTON2_IRQ,
        IRQ_Key_Interrupts2,
        0x0,
        0x0);
    //button3
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON3_BASE, 1); // 使能中断
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON3_BASE, 1); // 清中断边沿检测寄存器
    alt_ic_isr_register(
        BUTTON3_IRQ_INTERRUPT_CONTROLLER_ID,
        BUTTON3_IRQ,
        IRQ_Key_Interrupts3,
        0x0,
        0x0);
}

void IRQ_Key_Interrupts1() {
    if (add_func == 1) {
        actual_time += 60;
        if (actual_time >= 86400) actual_time = 0;
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON1_BASE, 1); // 清中断边沿检测寄存器
    } else if (add_func == 2) {
        alarm_time += 60;
        if (alarm_time >= 86400) alarm_time = 0;
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON1_BASE, 1);
    }
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON1_BASE, 1);
    DISP(actual_time);
}

void IRQ_Key_Interrupts2() {
    if (add_func == 1) {
        actual_time += 3600;
        if (actual_time >= 86400) actual_time = 0;
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON2_BASE, 1);
    } else if (add_func == 2) {
        alarm_time += 3600;
        if (alarm_time >= 86400) alarm_time = 0;
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON2_BASE, 1);
    }
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON2_BASE, 1);
    DISP(actual_time);
}
void IRQ_Key_Interrupts3() {
    if (add_func == 1) {
        actual_time ++;
        if (actual_time >= 86400) actual_time = 0;
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON3_BASE, 1);
    } else if (add_func == 2) {
        alarm_time ++;
        if (alarm_time >= 86400) alarm_time = 0;
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON3_BASE, 1);
    }
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON3_BASE, 1);
    DISP(actual_time);
}

void DISP(int time) { // 显示函数
    int display = 0;
    display = table[time / 36000] << 24 |
              table[(time % 36000) / 3600] << 16 |
              table[(time % 3600) / 600] << 8 |
              table[(time % 600 / 60)];
    IOWR_ALTERA_AVALON_PIO_DATA(DISPLAY_BASE, display);

    int display_2 = 0;
    display_2 = table[time % 60 / 10] << 8 |
                table[time % 10];
    IOWR_ALTERA_AVALON_PIO_DATA(DISPLAY_2_BASE, display_2);
}

// 定时器中断初始化
void Timer_Initial(void) {
    void* isr_context_ptr = (void*) &timer_isr_context;

    // 设置定时器周期
    // PERIODH << 16 | PERIODL = 计数器周期因子 * 时钟频率 - 1
    // 60s * 100M - 1 = 9999999 = 0x05F5E0FF
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x05F5);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0xE0FF);

    // 设置控制寄存器
    // 位 3：STOP；位 2：START；位 1：CONT；位 0：ITO
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE,
        ALTERA_AVALON_TIMER_CONTROL_START_MSK |
        ALTERA_AVALON_TIMER_CONTROL_CONT_MSK  |
        ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);

    alt_ic_isr_register(
        TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID,
        TIMER_0_IRQ,
        Timer_ISR,
        isr_context_ptr,
        0x0);
}
void Timer_Start(void) {
    IOWR_ALTERA_AVALON_TIMER_CONTROL(
        TIMER_0_BASE,
        ALTERA_AVALON_TIMER_CONTROL_START_MSK |
        ALTERA_AVALON_TIMER_CONTROL_CONT_MSK  |
        ALTERA_AVALON_TIMER_CONTROL_ITO_MSK
    );
}

// 停止定时器
void Timer_Stop(void) {
    IOWR_ALTERA_AVALON_TIMER_CONTROL(
        TIMER_0_BASE,
        ALTERA_AVALON_TIMER_CONTROL_STOP_MSK
    );
}
// 定时器中断服务子程序
void Timer_ISR(void* timer_isr_context) {
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE,
        ~ALTERA_AVALON_TIMER_STATUS_TO_MSK); // 清中断标志
    actual_time++; // 实时时间自增
   if (dis_flag == 0)
	   DISP(actual_time);
    sprintf((char*)lcd_alarm_time,"Alarm:%02u:%02u:%02u",alarm_time/3600,alarm_time%3600/60,alarm_time %60);
    select_xy(1,0);lcd_prints(lcd_alarm_time);
}

void delay(int count) {
    int i;
    for (i = 1; i <= count; i++);
}

void check_busy() { // 检查 LCD 忙标志
    delay(8000);
    // 可用时打开注释进行状态查询
    // alt_u8 status;
    // do {
    //     status = IORD_ALTERA_AVALON_LCD_16207_STATUS(LCD_16207_0_BASE);
    // } while (status & 0x80);
}

void lcd_init() { // LCD 1602 初始化
    usleep(400000);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38);
    usleep(5000);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38);
    usleep(5000);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38);
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38); // 功能设置
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x01); // 清屏
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x06); // 地址自增模式
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x0c); // 显示开，光标关
}

void lcd_prints(alt_u8 *string) { // 打印字符串
    while (*string != '\0') {
        check_busy();
        IOWR_ALTERA_AVALON_LCD_16207_DATA(LCD_16207_0_BASE, *string);
        string++;
    }
}

void select_xy(alt_u8 x, alt_u8 y) { // 选择 LCD 坐标，x=0 为第1行，x=1为第2行
    check_busy();
    if (x % 2 == 0) {
        IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x80 + y);
    } else {
        IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0xc0 + y);
    }
}



