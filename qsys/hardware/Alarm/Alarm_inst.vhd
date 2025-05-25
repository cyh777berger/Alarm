	component Alarm is
		port (
			button1_export   : in    std_logic                     := 'X';             -- export
			button2_export   : in    std_logic                     := 'X';             -- export
			button3_export   : in    std_logic                     := 'X';             -- export
			clk_clk          : in    std_logic                     := 'X';             -- clk
			display_export   : out   std_logic_vector(31 downto 0);                    -- export
			display_2_export : out   std_logic_vector(15 downto 0);                    -- export
			lcd_16207_RS     : out   std_logic;                                        -- RS
			lcd_16207_RW     : out   std_logic;                                        -- RW
			lcd_16207_data   : inout std_logic_vector(7 downto 0)  := (others => 'X'); -- data
			lcd_16207_E      : out   std_logic;                                        -- E
			ledg_export      : out   std_logic;                                        -- export
			ledr_export      : out   std_logic_vector(1 downto 0);                     -- export
			reset_reset_n    : in    std_logic                     := 'X';             -- reset_n
			sliders_export   : in    std_logic_vector(3 downto 0)  := (others => 'X')  -- export
		);
	end component Alarm;

	u0 : component Alarm
		port map (
			button1_export   => CONNECTED_TO_button1_export,   --   button1.export
			button2_export   => CONNECTED_TO_button2_export,   --   button2.export
			button3_export   => CONNECTED_TO_button3_export,   --   button3.export
			clk_clk          => CONNECTED_TO_clk_clk,          --       clk.clk
			display_export   => CONNECTED_TO_display_export,   --   display.export
			display_2_export => CONNECTED_TO_display_2_export, -- display_2.export
			lcd_16207_RS     => CONNECTED_TO_lcd_16207_RS,     -- lcd_16207.RS
			lcd_16207_RW     => CONNECTED_TO_lcd_16207_RW,     --          .RW
			lcd_16207_data   => CONNECTED_TO_lcd_16207_data,   --          .data
			lcd_16207_E      => CONNECTED_TO_lcd_16207_E,      --          .E
			ledg_export      => CONNECTED_TO_ledg_export,      --      ledg.export
			ledr_export      => CONNECTED_TO_ledr_export,      --      ledr.export
			reset_reset_n    => CONNECTED_TO_reset_reset_n,    --     reset.reset_n
			sliders_export   => CONNECTED_TO_sliders_export    --   sliders.export
		);

