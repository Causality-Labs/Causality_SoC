`timescale 1ns / 1ps

module testbench;

  // Testbench signals
  reg CLK;
  reg [7:0] COLOUR_IN;
  wire [7:0] cout;
  wire hs;
  wire vs;
  wire [9:0] addrh; 
  wire [9:0] addrv;

  // Instantiate the DUT (Device Under Test)
  VGAInterface uut (
    .CLK(CLK),
    .COLOUR_IN(COLOUR_IN),
    .cout(cout),
    .hs(hs),
    .vs(vs),
    .addrh(addrh),
    .addrv(addrv)
  );

    initial begin
        COLOUR_IN = 8'hFF;
    end

endmodule
