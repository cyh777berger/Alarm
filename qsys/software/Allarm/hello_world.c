#include "system.h" // ϵͳͷ�ļ�
#include "altera_avalon_timer_regs.h" // ��ʱ��ͷ�ļ�
#include "altera_avalon_pio_regs.h" // PIO ͷ�ļ�
#include "sys/alt_irq.h" // �ж�ͷ�ļ�
#include "unistd.h" // usleep ��ʱ����ͷ�ļ�
#include "alt_types.h"
#include <stdio.h> // ��׼�������ͷ�ļ�
#include <altera_avalon_lcd_16207_regs.h> // LCD ����ͷ�ļ�
#include <math.h>  // [MATRIX] ����������Ҫ

/**/
#define MAX_ITER 100       // ���ӵ�������
#define TOLERANCE 1e-7f    // ��߾���Ҫ��
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX_SIZE 20           // ���֧�־���ߴ磨����FPGA��Դ������

alt_u8 mul_flag;
alt_u8 dis_flag=0;
//����ṹ�壨��̬�ڴ棩
typedef struct {
    int rows;
    int cols;
    float data[MAX_SIZE][MAX_SIZE];
} Matrix;

//����ӷ�
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

//�������
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

//����˷�
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

//��������
int matrix_inverse(const Matrix *input, Matrix *output)
{
    if (input->rows != input->cols) return -1;
    int n = input->rows;
    float mat[MAX_SIZE][2*MAX_SIZE] = {0};

    // ����������� [I|A]
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[i][j] = input->data[i][j];
            mat[i][j + n] = (i == j) ? 1 : 0;
        }
    }

    // ��˹��Ԫ
    for (int pivot = 0; pivot < n; pivot++)
    {
        if (fabs(mat[pivot][pivot]) < 1e-6) return -2; // �������

        // ��һ������
        float scale = mat[pivot][pivot];
        for (int j = pivot; j < 2*n; j++)
        {
            mat[pivot][j] /= scale;
        }

        // ��ȥ������
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

    // ��ȡ�����
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

    // ��ʼ��S����ΪA�ĸ���
    S->rows = m; S->cols = n;
    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            S->data[i][j] = A->data[i][j];

    // ��ʼ��U��VΪ��λ����
    U->rows = U->cols = m;
    V->rows = V->cols = n;
    for(int i=0; i<MAX_SIZE; i++){
        for(int j=0; j<MAX_SIZE; j++){
            U->data[i][j] = (i==j && i<m) ? 1.0f : 0.0f;
            V->data[i][j] = (i==j && i<n) ? 1.0f : 0.0f;
        }
    }

    // �Ľ����ſɱȵ���
    for(int iter=0; iter<MAX_ITER; iter++){
        float max_off = 0.0f;
        int p = 0, q = 0;

        // Ѱ�����ǶԽ�Ԫ�أ�ȫ����ɨ�裩
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

        // ����2x2�Ӿ���
        float a = S->data[p][p], b = S->data[p][q];
        float c = S->data[q][p], d = S->data[q][q];

        // ���㾫ȷ��ת�Ƕȣ�˫���ſɱȣ�
        float theta = 0.5f * atan2(2*(b + c), a - d);
        float cos_theta = cosf(theta);
        float sin_theta = sinf(theta);

        // �����ת�����в�����
        float temp_row[MAX_SIZE];
        for(int j=0; j<n; j++){
            temp_row[j] = S->data[p][j];
            S->data[p][j] =  cos_theta * temp_row[j] + sin_theta * S->data[q][j];
            S->data[q][j] = -sin_theta * temp_row[j] + cos_theta * S->data[q][j];
        }

        // ����U����
        for(int i=0; i<m; i++){
            float Uip = U->data[i][p];
            float Uiq = U->data[i][q];
            U->data[i][p] =  cos_theta * Uip + sin_theta * Uiq;
            U->data[i][q] = -sin_theta * Uip + cos_theta * Uiq;
        }

        // �ҳ���ת�����в�����
        float temp_col[MAX_SIZE];
        for(int i=0; i<m; i++){
            temp_col[i] = S->data[i][p];
            S->data[i][p] =  cos_theta * temp_col[i] + sin_theta * S->data[i][q];
            S->data[i][q] = -sin_theta * temp_col[i] + cos_theta * S->data[i][q];
        }

        // ����V����
        for(int j=0; j<n; j++){
            float Vjp = V->data[j][p];
            float Vjq = V->data[j][q];
            V->data[j][p] =  cos_theta * Vjp + sin_theta * Vjq;
            V->data[j][q] = -sin_theta * Vjp + cos_theta * Vjq;
        }
    }

    // ȷ������ֵ�Ǹ�
    for(int i=0; i<size; i++){
        if(S->data[i][i] < 0){
            S->data[i][i] = -S->data[i][i];
            for(int j=0; j<m; j++)
                U->data[j][i] = -U->data[j][i];
        }
    }

    // ����ֵ�����ȶ�ð������
    for(int i=0; i<size; i++){
        for(int j=i+1; j<size; j++){
            if(S->data[i][i] < S->data[j][j]){
                // ��������ֵ
                float s_val = S->data[i][i];
                S->data[i][i] = S->data[j][j];
                S->data[j][j] = s_val;

                // ����U����
                for(int k=0; k<m; k++){
                    float tmp = U->data[k][i];
                    U->data[k][i] = U->data[k][j];
                    U->data[k][j] = tmp;
                }

                // ����V����
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
            printf("%8.3f ", mat->data[i][j]); // ͨ��JTAG UART���
        }
        printf("\n");
    }
}

/**/
// Hex �� 7 �������ת����
unsigned char table[16] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F};

// ��������
void IRQ_Init(); // �жϳ�ʼ������
void IRQ_Key_Interrupts1(); // �жϷ����ӳ��򣨰�ť1��
void IRQ_Key_Interrupts2(); // �жϷ����ӳ��򣨰�ť2��
void IRQ_Key_Interrupts3(); // �жϷ����ӳ��򣨰�ť3��
void DISP(int time); // ��ʾ����
void Timer_Initial(void); // ��ʼ����ʱ���ж�
void Timer_ISR(void* isr_context); // ��ʱ���жϷ�����
void Timer_Start(void);
void Timer_Stop(void);
void check_busy(); // ��� LCD æ״̬
void lcd_init(); // LCD ��ʼ��
void delay(); // ��ʱ����
void lcd_prints(alt_u8 *string); 	// ��ӡ�ַ����� LCD
void select_xy(alt_u8 x, alt_u8 y); // ѡ�� LCD ���λ��

// ȫ�ֱ���
alt_u32 timer_isr_context; // ����ȫ�ֱ������ڴ洢 isr_context ָ��
alt_u8 add_func = 0; // Ĭ��Ϊ�����ã�1�������õ�ǰʱ�䣬2������������ʱ��
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
alt_u8 matrix_done = 0;  // [MATRIX] ��Ǿ����Ƿ���ִ��

// ������
int main(void) {
    IRQ_Init(); // ��ʼ�� PIO �ж�
    lcd_init(); // LCD ��ʼ��
    select_xy(0, 0);
    lcd_prints(lcd_string1);
    Timer_Initial(); // ��ʼ����ʱ���ж�
    IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x0); // ��ʼ�� LEDR ȫ��
	Matrix A, B, C, invA, U, S, V;
    while (1) {
    	 // ��ȡ sw3 ����ģʽ
    	    if ((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE) & 0x8) != 0)
    	        mode = 1; // ��������ģʽ
    	     else
    	        mode = 0; // ����ģʽ
    	    if (mode == 0)
    	    {
    	    	//ʱ�䵽����LCD
					if(actual_time-alarm_time>5)
					{
						i1=1;
						lcd_active=0;
						i3=0;
					}
					else i1=i2=0;
					if(lcd_active==0&&i1==1&&i2==0)
					{
						IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x01);   //����
						usleep(5000);
						select_xy(0, 0);
						lcd_prints(lcd_string1);

						i2=1;
					}
					if(lcd_flag==1 && lcd_active==1)//���ʱ�䵽�ˣ�LCD��ʾTIME
					{
						IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x01);   //����
						usleep(5000);
						select_xy(0,0);
						lcd_prints(lcd_string2);
						lcd_flag=0;
						lcd_active=0;
					}
    	    	        // ʱ�䵽�������
    	    	        if (alarm_active != 0)
    	    	        {
    	    	            IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x3); // ���ʱ�䵽������� 5 ��
    	    	            lcd_flag = 1; // LCD ��ʾ
    	    	        }
    	    	        else
    	    	           IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x0);
    	    	        // ���� sliders ״̬���
					if (((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE)) & 1) != 0)
					{ // ���ӿ��ش�
						IOWR_ALTERA_AVALON_PIO_DATA(LEDG_BASE, 0x1); // �̵ƿ�
						if (((actual_time - alarm_time) >= 0) && (actual_time - alarm_time) <= 5)
						{
							alarm_active = 1; // ����� 5 ��
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
						IOWR_ALTERA_AVALON_PIO_DATA(LEDG_BASE, 0x0); // �̵ƹ�
						alarm_active = 0;
    	    	    }

    	    	        // ���õ�ǰʱ��
    	    	        if (((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE)) & 4) != 0) {
    	    	           // DISP(actual_time);
    	    	        	Timer_Stop();
    	    	            add_func = 1; // button1 �ж�����
    	    	        }
    	    	        // ��������ʱ��
    	    	        else if (((IORD_ALTERA_AVALON_PIO_DATA(SLIDERS_BASE)) & 2) != 0) {
    	    	            DISP(alarm_time);
    	    	            //Timer_Stop();
    	    	            dis_flag=1;
    	    	            add_func = 2; // button2 �ж�����
    	    	        } else {
    	    	            //DISP(actual_time);
    	    	        	Timer_Start();
    	    	            add_func = 0;
    	    	            dis_flag=0;
    	    	        }
    	       } else if (mode == 1 && matrix_done == 0) {
    	           matrix_done = 1;
    	           //�������A
    	           	    printf("\n���������A������������: ");
    	           	    scanf("%d %d", &A.rows, &A.cols);
    	           	    printf("�밴���������A�� %d ��Ԫ�أ��ÿո�ָ���:\n", A.rows*A.cols);
    	           	    for(int i=0; i<A.rows; i++)
    	           	    {
    	           	        for(int j=0; j<A.cols; j++)
    	           	        {
    	           	            scanf("%f", &A.data[i][j]);
    	           	        }
    	           	    }
    	           	//�������B
    	           	    printf("\n���������B������������: ");
    	           	    scanf("%d %d", &B.rows, &B.cols);
    	           	    printf("�밴���������B�� %d ��Ԫ�أ��ÿո�ָ���:\n", B.rows*B.cols);
    	           	    for(int i=0; i<B.rows; i++)
    	           	    {
    	           	        for(int j=0; j<B.cols; j++)
    	           	        {
    	           	           scanf("%f", &B.data[i][j]);
    	           	        }
    	           	    }
    	               //����ӷ�
    	           	    matrix_add(&A, &B, &C);
    	           	    printf("\n����ӷ����\n");
    	           	    matrix_print(&C);
    	           	//�������
    	           	    matrix_sub(&A, &B, &C);
    	           	    printf("\n����������\n");
    	           	    matrix_print(&C);
    	           	//����˷�
    	           		matrix_multiply(&A, &B, &C);
    	           		printf("\n����˷����:\n");
    	           		if(mul_flag==0) printf("\nError!\n");
    	           		if(mul_flag==1) matrix_print(&C);
    	           	//��������
    	           		printf("\n����������\n");
    	           		if (matrix_inverse(&A, &invA) == 0)	matrix_print(&invA);
    	           		else printf("\nError!\n");
    	           	//����SVD�ֽ�
    	           		svd_decomposition(&A, &U, &S, &V);
    	           		printf("\n����SVD�ֽ�����\n");
    	           		printf("\nU Matrix:\n");
    	           		matrix_print(&U);
    	           		printf("\nS Matrix:\n");
    	           		matrix_print(&S);
    	           		printf("\nV Matrix:\n");
    	           		matrix_print(&V);
    	       }
    }
}

void IRQ_Init() { // ��ʼ�� PIO �ж�
    // button1
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON1_BASE, 1); // ʹ���ж�
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON1_BASE, 1); // ���жϱ��ؼ��Ĵ���
    alt_ic_isr_register(
        BUTTON1_IRQ_INTERRUPT_CONTROLLER_ID, // �жϿ����� ID
        BUTTON1_IRQ, // Ӳ���жϺ�
        IRQ_Key_Interrupts1, // �жϷ�����
        0x0, // ISR ������ָ��
        0x0); // flags������δ��

    // button2
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON2_BASE, 1); // ʹ���ж�
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON2_BASE, 1); // ���жϱ��ؼ��Ĵ���
    alt_ic_isr_register(
        BUTTON2_IRQ_INTERRUPT_CONTROLLER_ID,
        BUTTON2_IRQ,
        IRQ_Key_Interrupts2,
        0x0,
        0x0);
    //button3
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON3_BASE, 1); // ʹ���ж�
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON3_BASE, 1); // ���жϱ��ؼ��Ĵ���
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
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON1_BASE, 1); // ���жϱ��ؼ��Ĵ���
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

void DISP(int time) { // ��ʾ����
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

// ��ʱ���жϳ�ʼ��
void Timer_Initial(void) {
    void* isr_context_ptr = (void*) &timer_isr_context;

    // ���ö�ʱ������
    // PERIODH << 16 | PERIODL = �������������� * ʱ��Ƶ�� - 1
    // 60s * 100M - 1 = 9999999 = 0x05F5E0FF
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x05F5);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0xE0FF);

    // ���ÿ��ƼĴ���
    // λ 3��STOP��λ 2��START��λ 1��CONT��λ 0��ITO
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

// ֹͣ��ʱ��
void Timer_Stop(void) {
    IOWR_ALTERA_AVALON_TIMER_CONTROL(
        TIMER_0_BASE,
        ALTERA_AVALON_TIMER_CONTROL_STOP_MSK
    );
}
// ��ʱ���жϷ����ӳ���
void Timer_ISR(void* timer_isr_context) {
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE,
        ~ALTERA_AVALON_TIMER_STATUS_TO_MSK); // ���жϱ�־
    actual_time++; // ʵʱʱ������
   if (dis_flag == 0)
	   DISP(actual_time);
    sprintf((char*)lcd_alarm_time,"Alarm:%02u:%02u:%02u",alarm_time/3600,alarm_time%3600/60,alarm_time %60);
    select_xy(1,0);lcd_prints(lcd_alarm_time);
}

void delay(int count) {
    int i;
    for (i = 1; i <= count; i++);
}

void check_busy() { // ��� LCD æ��־
    delay(8000);
    // ����ʱ��ע�ͽ���״̬��ѯ
    // alt_u8 status;
    // do {
    //     status = IORD_ALTERA_AVALON_LCD_16207_STATUS(LCD_16207_0_BASE);
    // } while (status & 0x80);
}

void lcd_init() { // LCD 1602 ��ʼ��
    usleep(400000);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38);
    usleep(5000);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38);
    usleep(5000);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38);
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x38); // ��������
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x01); // ����
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x06); // ��ַ����ģʽ
    usleep(5000);
    check_busy();
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x0c); // ��ʾ��������
}

void lcd_prints(alt_u8 *string) { // ��ӡ�ַ���
    while (*string != '\0') {
        check_busy();
        IOWR_ALTERA_AVALON_LCD_16207_DATA(LCD_16207_0_BASE, *string);
        string++;
    }
}

void select_xy(alt_u8 x, alt_u8 y) { // ѡ�� LCD ���꣬x=0 Ϊ��1�У�x=1Ϊ��2��
    check_busy();
    if (x % 2 == 0) {
        IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0x80 + y);
    } else {
        IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_16207_0_BASE, 0xc0 + y);
    }
}



