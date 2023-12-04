// new_component.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module new_component (
		input  wire        clk,                  //          clk.clk
		input  wire        rst_reset,            //          rst.reset
		input  wire        avalon_slave_address, // avalon_slave.address
		input  wire        avalon_slave_write,   //             .write
		input  wire [15:0] speed,                //             .writedata
		output wire [15:0] rpm_out,              //             .readdata
		input  wire        avalon_slave_read,    //             .read
		output wire [1:0]  pwm_out,              //      pwm_out.new_signal
		input  wire [1:0]  encoder_in            //   encoder_in.new_signal
	);

	// TODO: Auto-generated HDL template
	reg [10:0] pwm_count;
    reg signed [10:0] abs_speed;
    reg [1:0] prev_encoder;
    reg [10:0] prev_edge_count;
	 reg [0:0] dir;
	 reg [15:0] rpm_hold;
	 reg [15:0] set_speed;

    always @(posedge clk or posedge rst_reset) begin
        if (rst_reset) begin
            pwm_count = 11'b0;
            abs_speed = 11'b0;
				rpm_hold = 16'b0;
            prev_encoder = 2'b0;
            prev_edge_count = 11'd0;
            dir = 1'b0;
        end else begin
		  
				if (avalon_slave_write) begin
					set_speed <= speed;
				end
            dir = set_speed[12];

            abs_speed = set_speed[12] ? (~set_speed[12:0] + 13'b1) : set_speed[12:0];

            if (abs_speed > 13'hFFF) begin
                abs_speed = 13'hFFF;
            end

            pwm_count = pwm_count + 1;

            if (encoder_in[0] & ~prev_encoder[0]) begin
                rpm_hold  = ((50000000 / (encoder_in[1] ? -1 : 1)) / prev_edge_count);
                prev_edge_count = 11'd0;
            end else begin
                prev_edge_count = prev_edge_count + 1;
            end
            prev_encoder = encoder_in;
        end
    end
	 
	 assign  pwm_out[0] = (dir) ? (pwm_count < abs_speed): 1'b0;
    assign  pwm_out[1] = (~dir) ? (pwm_count < abs_speed) : 1'b0;
	 assign rpm_out = rpm_hold;

endmodule