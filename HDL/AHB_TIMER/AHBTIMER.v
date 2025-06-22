//////////////////////////////////////////////////////////////////////////////////
//END USER LICENCE AGREEMENT                                                    //
//                                                                              //
//Copyright (c) 2012, ARM All rights reserved.                                  //
//                                                                              //
//THIS END USER LICENCE AGREEMENT (�LICENCE�) IS A LEGAL AGREEMENT BETWEEN      //
//YOU AND ARM LIMITED ("ARM") FOR THE USE OF THE SOFTWARE EXAMPLE ACCOMPANYING  //
//THIS LICENCE. ARM IS ONLY WILLING TO LICENSE THE SOFTWARE EXAMPLE TO YOU ON   //
//CONDITION THAT YOU ACCEPT ALL OF THE TERMS IN THIS LICENCE. BY INSTALLING OR  //
//OTHERWISE USING OR COPYING THE SOFTWARE EXAMPLE YOU INDICATE THAT YOU AGREE   //
//TO BE BOUND BY ALL OF THE TERMS OF THIS LICENCE. IF YOU DO NOT AGREE TO THE   //
//TERMS OF THIS LICENCE, ARM IS UNWILLING TO LICENSE THE SOFTWARE EXAMPLE TO    //
//YOU AND YOU MAY NOT INSTALL, USE OR COPY THE SOFTWARE EXAMPLE.                //
//                                                                              //
//ARM hereby grants to you, subject to the terms and conditions of this Licence,//
//a non-exclusive, worldwide, non-transferable, copyright licence only to       //
//redistribute and use in source and binary forms, with or without modification,//
//for academic purposes provided the following conditions are met:              //
//a) Redistributions of source code must retain the above copyright notice, this//
//list of conditions and the following disclaimer.                              //
//b) Redistributions in binary form must reproduce the above copyright notice,  //
//this list of conditions and the following disclaimer in the documentation     //
//and/or other materials provided with the distribution.                        //
//                                                                              //
//THIS SOFTWARE EXAMPLE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ARM     //
//EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING     //
//WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR //
//PURPOSE, WITH RESPECT TO THIS SOFTWARE EXAMPLE. IN NO EVENT SHALL ARM BE LIABLE/
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES OF ANY/
//KIND WHATSOEVER WITH RESPECT TO THE SOFTWARE EXAMPLE. ARM SHALL NOT BE LIABLE //
//FOR ANY CLAIMS, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, //
//TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE    //
//EXAMPLE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE EXAMPLE. FOR THE AVOIDANCE/
// OF DOUBT, NO PATENT LICENSES ARE BEING LICENSED UNDER THIS LICENSE AGREEMENT.//
//////////////////////////////////////////////////////////////////////////////////


module AHBTIMER(
	//Inputs
	input wire HCLK,
	input wire HRESETn,
  input wire [31:0] HADDR,
  input wire [31:0] HWDATA,
  input wire [1:0] HTRANS,
  input wire HWRITE,
  input wire HSEL,
  input wire HREADY,
  
	//Output
  output wire [31:0] HRDATA,
	output wire HREADYOUT,
  output reg timer_irq
);

  localparam [7:0] LDADDR = 8'h00;   //load register address
  localparam [7:0] VALADDR = 8'h04;  //value register address
  localparam [7:0] CTLADDR = 8'h08;  //control register address
  localparam [7:0] CLRADDR = 8'h0C;  //clear register address
  localparam [7:0] CMPADDR = 8'h10;

  localparam [2:0] FREE_RUN = 3'b000;
  localparam [2:0] PERIODIC = 3'b001;
  localparam [2:0] COMPARE  = 3'b010;
  localparam [2:0] PWM      = 3'b011;

  localparam st_idle = 1'b0;
  localparam st_count = 1'b1;
  
  reg current_state;
  reg next_state;
  
  reg [31:0] value_next;
  reg timer_irq_next;
  
  //AHB Registers
  reg last_HWRITE;
  reg [31:0] last_HADDR;
  reg last_HSEL;
  reg [1:0] last_HTRANS;

  //internal registers
  reg [4:0] control;
  reg [31:0] load;
  reg clear;
  reg [31:0] value;
  reg [31:0] cmp_val;

  wire enable;
  wire [2:0] mode;

  //Prescaled clk signals
  wire clk16;       // HCLK/16
  wire timerclk;

  //Generate prescaled clk ticks
  prescaler uprescaler16(
    .inclk(HCLK),
    .outclk(clk16)
  );

  
  assign HREADYOUT = 1'b1; //Always ready

  always @(posedge HCLK)
    if(HREADY)
      begin
        last_HWRITE <= HWRITE;
        last_HSEL <= HSEL;
        last_HADDR <= HADDR;
        last_HTRANS <= HTRANS;
      end
  
  // Sample Control value = 0x11,10001 ->  clk16:FREE_RUN:enable  
  // Sample Control value = 0x13,10011 ->  clk16:PERIODIC:enable
  // Sample Control value = 0x15,10101 ->  clk16:COMPARE:enable
  // Sample Control value = 0x17,10111 ->  clk16:PWM:enable
  //Prescale clk based on control[2]  01 = 16 ; 00 = 1;
  assign timerclk = ((control[4]) ? clk16 : 1'b1);  //1'b1 signifies HCLK
                      
  assign enable = control[0];
  assign mode = (control[3:1] == FREE_RUN) ? FREE_RUN :
                (control[3:1] == PERIODIC) ? PERIODIC :
                (control[3:1] == COMPARE)  ? COMPARE  :
                (control[3:1] == PWM)      ? PWM      :
                FREE_RUN;
  
  //Control signal
  always @(posedge HCLK, negedge HRESETn)
    if(!HRESETn)
      control <= 5'b00000;
    else if(last_HWRITE & last_HSEL & last_HTRANS[1])
      if(last_HADDR[7:0] == CTLADDR)
        control <= HWDATA[4:0];
        
        
  //Load signal
  always @(posedge HCLK, negedge HRESETn)
    if(!HRESETn)
      load <= 32'h0000_0000;
    else if(last_HWRITE & last_HSEL & last_HTRANS[1])
      if(last_HADDR[7:0] == LDADDR)
        load <= HWDATA;

  //Compare signal
  always @(posedge HCLK, negedge HRESETn)
    if(!HRESETn)
      cmp_val <= 32'h0000_0000;
    else if(last_HWRITE & last_HSEL & last_HTRANS[1])
      if(last_HADDR[7:0] == CMPADDR)
        cmp_val <= HWDATA;

   //Clear signal      
  always @(posedge HCLK, negedge HRESETn)
    if(!HRESETn)
      clear <= 1'b0;
    else if(last_HWRITE & last_HSEL & last_HTRANS[1])
      if(last_HADDR[7:0] == CLRADDR)
        clear <= HWDATA[0];

  
  always @(posedge HCLK, negedge HRESETn)
    if(!HRESETn)
      timer_irq <= 1'b0;
    else
      timer_irq <= timer_irq_next;
           
  //State Machine    
  always @(posedge HCLK, negedge HRESETn)
    if(!HRESETn)
      begin
        current_state <= st_idle;
        value <= 32'h0000_0000;
      end
    else
      begin
        value <= value_next;
        current_state <= next_state;
      end
  
  //Timer Operation and Next State logic
  always @*
  begin
    next_state = current_state;
    value_next = value;
    timer_irq_next = (clear) ? 0 : timer_irq;
    case(current_state)
      st_idle:
        if(enable && timerclk)
            begin
              value_next = load;
              next_state = st_count;
            end

      st_count:
        if(enable && timerclk)
        begin
          case(mode)
            FREE_RUN: begin
                        if (value == 32'h0000_0000)
                          begin
                            timer_irq_next = 1;
                          end
                        value_next = value - 1;
                      end
            PERIODIC: begin
                        if (value == 32'h0000_0000)
                          begin
                            timer_irq_next = 1;
                            value_next = load;
                          end
                        else
                          value_next = value - 1;
                      end
            
            COMPARE: begin
                        if (value == cmp_val)
                          begin
                            timer_irq_next = 1;
                            value_next = load;
                          end
                        else
                          value_next = value - 1;
                      end
            PWM: begin
                   if (value == cmp_val)
                     begin
                       timer_irq_next = 1;
                       value_next = load;
                     end
                   else
                     value_next = value - 1;
                 end
            default: begin
                       // Optionally define a default behavior.
                       value_next = value - 1;
                     end
          endcase
        end
    endcase
  end
  
  
  assign HRDATA = (last_HADDR[7:0] == LDADDR) ? load :
                  (last_HADDR[7:0] == VALADDR) ? value :
                  (last_HADDR[7:0] == CTLADDR) ? control :
                   32'h0000_0000;
            


endmodule
