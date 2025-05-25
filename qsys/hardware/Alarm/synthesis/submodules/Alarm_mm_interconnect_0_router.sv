// (C) 2001-2018 Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files from any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License Subscription 
// Agreement, Intel FPGA IP License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Intel and sold by 
// Intel or its authorized distributors.  Please refer to the applicable 
// agreement for further details.



// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// $Id: //acds/rel/18.1std/ip/merlin/altera_merlin_router/altera_merlin_router.sv.terp#1 $
// $Revision: #1 $
// $Date: 2018/07/18 $
// $Author: psgswbuild $

// -------------------------------------------------------
// Merlin Router
//
// Asserts the appropriate one-hot encoded channel based on 
// either (a) the address or (b) the dest id. The DECODER_TYPE
// parameter controls this behaviour. 0 means address decoder,
// 1 means dest id decoder.
//
// In the case of (a), it also sets the destination id.
// -------------------------------------------------------

`timescale 1 ns / 1 ns

module Alarm_mm_interconnect_0_router_default_decode
  #(
     parameter DEFAULT_CHANNEL = 4,
               DEFAULT_WR_CHANNEL = -1,
               DEFAULT_RD_CHANNEL = -1,
               DEFAULT_DESTID = 10 
   )
  (output [84 - 81 : 0] default_destination_id,
   output [15-1 : 0] default_wr_channel,
   output [15-1 : 0] default_rd_channel,
   output [15-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[84 - 81 : 0];

  generate
    if (DEFAULT_CHANNEL == -1) begin : no_default_channel_assignment
      assign default_src_channel = '0;
    end
    else begin : default_channel_assignment
      assign default_src_channel = 15'b1 << DEFAULT_CHANNEL;
    end
  endgenerate

  generate
    if (DEFAULT_RD_CHANNEL == -1) begin : no_default_rw_channel_assignment
      assign default_wr_channel = '0;
      assign default_rd_channel = '0;
    end
    else begin : default_rw_channel_assignment
      assign default_wr_channel = 15'b1 << DEFAULT_WR_CHANNEL;
      assign default_rd_channel = 15'b1 << DEFAULT_RD_CHANNEL;
    end
  endgenerate

endmodule


module Alarm_mm_interconnect_0_router
(
    // -------------------
    // Clock & Reset
    // -------------------
    input clk,
    input reset,

    // -------------------
    // Command Sink (Input)
    // -------------------
    input                       sink_valid,
    input  [98-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [98-1    : 0] src_data,
    output reg [15-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 55;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 84;
    localparam PKT_DEST_ID_L = 81;
    localparam PKT_PROTECTION_H = 88;
    localparam PKT_PROTECTION_L = 86;
    localparam ST_DATA_W = 98;
    localparam ST_CHANNEL_W = 15;
    localparam DECODER_TYPE = 0;

    localparam PKT_TRANS_WRITE = 58;
    localparam PKT_TRANS_READ  = 59;

    localparam PKT_ADDR_W = PKT_ADDR_H-PKT_ADDR_L + 1;
    localparam PKT_DEST_ID_W = PKT_DEST_ID_H-PKT_DEST_ID_L + 1;



    // -------------------------------------------------------
    // Figure out the number of bits to mask off for each slave span
    // during address decoding
    // -------------------------------------------------------
    localparam PAD0 = log2ceil(64'h80000 - 64'h40000); 
    localparam PAD1 = log2ceil(64'hc0000 - 64'ha0000); 
    localparam PAD2 = log2ceil(64'hc1000 - 64'hc0800); 
    localparam PAD3 = log2ceil(64'hc1020 - 64'hc1000); 
    localparam PAD4 = log2ceil(64'hc1030 - 64'hc1020); 
    localparam PAD5 = log2ceil(64'hc1040 - 64'hc1030); 
    localparam PAD6 = log2ceil(64'hc1050 - 64'hc1040); 
    localparam PAD7 = log2ceil(64'hc1060 - 64'hc1050); 
    localparam PAD8 = log2ceil(64'hc1070 - 64'hc1060); 
    localparam PAD9 = log2ceil(64'hc1080 - 64'hc1070); 
    localparam PAD10 = log2ceil(64'hc1090 - 64'hc1080); 
    localparam PAD11 = log2ceil(64'hc10a0 - 64'hc1090); 
    localparam PAD12 = log2ceil(64'hc10b0 - 64'hc10a0); 
    localparam PAD13 = log2ceil(64'hc10b8 - 64'hc10b0); 
    localparam PAD14 = log2ceil(64'hc10c0 - 64'hc10b8); 
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'hc10c0;
    localparam RANGE_ADDR_WIDTH = log2ceil(ADDR_RANGE);
    localparam OPTIMIZED_ADDR_H = (RANGE_ADDR_WIDTH > PKT_ADDR_W) ||
                                  (RANGE_ADDR_WIDTH == 0) ?
                                        PKT_ADDR_H :
                                        PKT_ADDR_L + RANGE_ADDR_WIDTH - 1;

    localparam RG = RANGE_ADDR_WIDTH-1;
    localparam REAL_ADDRESS_RANGE = OPTIMIZED_ADDR_H - PKT_ADDR_L;

      reg [PKT_ADDR_W-1 : 0] address;
      always @* begin
        address = {PKT_ADDR_W{1'b0}};
        address [REAL_ADDRESS_RANGE:0] = sink_data[OPTIMIZED_ADDR_H : PKT_ADDR_L];
      end   

    // -------------------------------------------------------
    // Pass almost everything through, untouched
    // -------------------------------------------------------
    assign sink_ready        = src_ready;
    assign src_valid         = sink_valid;
    assign src_startofpacket = sink_startofpacket;
    assign src_endofpacket   = sink_endofpacket;
    wire [PKT_DEST_ID_W-1:0] default_destid;
    wire [15-1 : 0] default_src_channel;




    // -------------------------------------------------------
    // Write and read transaction signals
    // -------------------------------------------------------
    wire read_transaction;
    assign read_transaction  = sink_data[PKT_TRANS_READ];


    Alarm_mm_interconnect_0_router_default_decode the_default_decode(
      .default_destination_id (default_destid),
      .default_wr_channel   (),
      .default_rd_channel   (),
      .default_src_channel  (default_src_channel)
    );

    always @* begin
        src_data    = sink_data;
        src_channel = default_src_channel;
        src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = default_destid;

        // --------------------------------------------------
        // Address Decoder
        // Sets the channel and destination ID based on the address
        // --------------------------------------------------

    // ( 0x40000 .. 0x80000 )
    if ( {address[RG:PAD0],{PAD0{1'b0}}} == 20'h40000   ) begin
            src_channel = 15'b000000000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 10;
    end

    // ( 0xa0000 .. 0xc0000 )
    if ( {address[RG:PAD1],{PAD1{1'b0}}} == 20'ha0000   ) begin
            src_channel = 15'b010000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 11;
    end

    // ( 0xc0800 .. 0xc1000 )
    if ( {address[RG:PAD2],{PAD2{1'b0}}} == 20'hc0800   ) begin
            src_channel = 15'b000000000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
    end

    // ( 0xc1000 .. 0xc1020 )
    if ( {address[RG:PAD3],{PAD3{1'b0}}} == 20'hc1000   ) begin
            src_channel = 15'b000100000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 14;
    end

    // ( 0xc1020 .. 0xc1030 )
    if ( {address[RG:PAD4],{PAD4{1'b0}}} == 20'hc1020   ) begin
            src_channel = 15'b100000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
    end

    // ( 0xc1030 .. 0xc1040 )
    if ( {address[RG:PAD5],{PAD5{1'b0}}} == 20'hc1030   ) begin
            src_channel = 15'b001000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
    end

    // ( 0xc1040 .. 0xc1050 )
    if ( {address[RG:PAD6],{PAD6{1'b0}}} == 20'hc1040   ) begin
            src_channel = 15'b000010000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 5;
    end

    // ( 0xc1050 .. 0xc1060 )
    if ( {address[RG:PAD7],{PAD7{1'b0}}} == 20'hc1050  && read_transaction  ) begin
            src_channel = 15'b000001000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 12;
    end

    // ( 0xc1060 .. 0xc1070 )
    if ( {address[RG:PAD8],{PAD8{1'b0}}} == 20'hc1060   ) begin
            src_channel = 15'b000000100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
    end

    // ( 0xc1070 .. 0xc1080 )
    if ( {address[RG:PAD9],{PAD9{1'b0}}} == 20'hc1070   ) begin
            src_channel = 15'b000000010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
    end

    // ( 0xc1080 .. 0xc1090 )
    if ( {address[RG:PAD10],{PAD10{1'b0}}} == 20'hc1080   ) begin
            src_channel = 15'b000000001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 8;
    end

    // ( 0xc1090 .. 0xc10a0 )
    if ( {address[RG:PAD11],{PAD11{1'b0}}} == 20'hc1090   ) begin
            src_channel = 15'b000000000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 9;
    end

    // ( 0xc10a0 .. 0xc10b0 )
    if ( {address[RG:PAD12],{PAD12{1'b0}}} == 20'hc10a0   ) begin
            src_channel = 15'b000000000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
    end

    // ( 0xc10b0 .. 0xc10b8 )
    if ( {address[RG:PAD13],{PAD13{1'b0}}} == 20'hc10b0  && read_transaction  ) begin
            src_channel = 15'b000000000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 13;
    end

    // ( 0xc10b8 .. 0xc10c0 )
    if ( {address[RG:PAD14],{PAD14{1'b0}}} == 20'hc10b8   ) begin
            src_channel = 15'b000000000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
    end

end


    // --------------------------------------------------
    // Ceil(log2()) function
    // --------------------------------------------------
    function integer log2ceil;
        input reg[65:0] val;
        reg [65:0] i;

        begin
            i = 1;
            log2ceil = 0;

            while (i < val) begin
                log2ceil = log2ceil + 1;
                i = i << 1;
            end
        end
    endfunction

endmodule


