
module Alarm (
	button1_export,
	button2_export,
	button3_export,
	clk_clk,
	display_export,
	display_2_export,
	lcd_16207_RS,
	lcd_16207_RW,
	lcd_16207_data,
	lcd_16207_E,
	ledg_export,
	ledr_export,
	reset_reset_n,
	sliders_export);	

	input		button1_export;
	input		button2_export;
	input		button3_export;
	input		clk_clk;
	output	[31:0]	display_export;
	output	[15:0]	display_2_export;
	output		lcd_16207_RS;
	output		lcd_16207_RW;
	inout	[7:0]	lcd_16207_data;
	output		lcd_16207_E;
	output		ledg_export;
	output	[1:0]	ledr_export;
	input		reset_reset_n;
	input	[3:0]	sliders_export;
endmodule
