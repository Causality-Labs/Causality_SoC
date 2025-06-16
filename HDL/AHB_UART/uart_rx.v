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


module UART_RX(
  input wire clk,
  input wire resetn,
  input wire b_tick,        //Baud generator tick
  input wire rx,            //RS-232 data port
  input wire parity_en,
  input wire parity_odd,
  output reg parity_err,
  output reg rx_done,       //transfer completed
  output wire [7:0] dout    //output data
  );

//STATE DEFINES  
  localparam [2:0] idle_st   = 3'b000;
  localparam [2:0] start_st  = 3'b001;
  localparam [2:0] data_st   = 3'b010;
  localparam [2:0] parity_st = 3'b011;
  localparam [2:0] stop_st   = 3'b100;

//Internal Signals  
  reg [2:0] current_state;
  reg [2:0] next_state;
  reg [3:0] b_reg; //baud-rate/over sampling counter
  reg [3:0] b_next;
  reg [2:0] count_reg; //data-bit counter
  reg [2:0] count_next;
  reg [7:0] data_reg; //data register
  reg [7:0] data_next;

  
//State Machine  
  always @ (posedge clk, negedge resetn)
  begin
    if(!resetn)
      begin
        current_state <= idle_st;
        b_reg <= 0;
        count_reg <= 0;
        data_reg <=0;
        parity_err <= 1'b0;
      end
    else
      begin
        current_state <= next_state;
        b_reg <= b_next;
        count_reg <= count_next;
        data_reg <= data_next;
      end
  end

//Next State Logic 
  always @*
  begin
    next_state = current_state;
    b_next = b_reg;
    count_next = count_reg;
    data_next = data_reg;
    rx_done = 1'b0;
    parity_err = 1'b0;
        
    case(current_state)
      idle_st:
        if(~rx)
          begin
            next_state = start_st;
            b_next = 0;
          end
          
      start_st:
        if(b_tick)
          if(b_reg == 7)
            begin
              next_state = data_st;
              b_next = 0;
              count_next = 0;
            end
          else
            b_next = b_reg + 1'b1;
            
      data_st:
        if(b_tick)
          if(b_reg == 15)
            begin
              b_next = 0;
              data_next = {rx, data_reg [7:1]};
              if(count_next == 7) // 8 Data bits
                next_state = parity_en ? parity_st : stop_st;
              else
                count_next = count_reg + 1'b1;
            end
          else
            b_next = b_reg + 1;

      parity_st: begin
        // In parity_st, sample the parity bit from rx after one bit period.
        if (b_tick) begin
          if (b_reg == 15) begin
            b_next = 0;
            // Compute expected parity from received data:
            // (^data_reg) computes even parity over data_reg bits.
            // If odd parity is required, invert the computed parity.
            if (parity_odd) begin
              if (rx != ~(^data_reg))
                parity_err = 1'b1;
            end else begin
              if (rx != (^data_reg))
                parity_err = 1'b1;
            end
            next_state = stop_st;
          end else begin
            b_next = b_reg + 1;
          end
        end
      end
            
      stop_st:
        if(b_tick)
          if(b_reg == 15) //One stop bit
            begin
              next_state = idle_st;
              rx_done = 1'b1;
            end
          else
           b_next = b_reg + 1;
    endcase
  end
  
  
  assign dout = data_reg;
  
  
  
endmodule
