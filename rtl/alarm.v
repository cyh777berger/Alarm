module alarm(

input sys_clk,
input sys_rst_n,

input pushbuttons1,
input pushbuttons2,
input pushbuttons3,
input [3:0]sliders,

output [1:0]ledr,
output ledg,
output [31:0]display,
output [15:0]display_2,
inout [7:0] LCD_DATA,
output LCD_ON,
output LCD_BLON,
output LCD_RW,
output LCD_EN,
output LCD_RS
);


wire clk_100m;
assign LCD_DATA = 8'hzz;
assign LCD_ON   = 1'b1;
assign LCD_BLON = 1'b1;


		pll  u_pll(
			.inclk0(sys_clk),
			.c0(clk_100m)

		);

    Alarm u0 (
        .button1_export   (pushbuttons1),   //   button1.export
        .button2_export   (pushbuttons2),   //   button2.export
        .button3_export   (pushbuttons3),   //   button3.export
        .clk_clk          (clk_100m),          //       clk.clk
        .display_export   (display),   //   display.export
		  .display_2_export (display_2), //   display.export
        .lcd_16207_RS   (LCD_RS),   	//   lcd_16207.RS
        .lcd_16207_RW   (LCD_RW),   	//          .RW
        .lcd_16207_data (LCD_DATA), 	//          .data
        .lcd_16207_E    (LCD_EN),    	//          .E
        .ledg_export    (ledg),    		//      ledg.export
        .ledr_export    (ledr),   		//      ledr.export
        .reset_reset_n  (sys_rst_n),  	//     reset.reset_n
        .sliders_export (sliders) 		//   sliders.export
    );


endmodule