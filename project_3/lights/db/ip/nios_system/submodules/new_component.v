module new_component (

    input wire clk_clk, // clock_sink.clk
    input wire rst_reset, // reset.reset
    input wire avalon_slave_address, // avalon_slave.address
    input wire [31:0] avalon_slave_writedata, // .writedata
    input wire avalon_slave_write, // .write
    output wire [25:0] leds // conduit_end.new_signal

);

 

    // Register to store the duty cycle values for led_hold

    reg [675:0] cnt;
    reg [675:0] pwm;
    reg [25:0] led_hold;

 

    always @(posedge clk_clk or posedge rst_reset) begin

        integer i;

        if (rst_reset) begin
            cnt <= 676'b0;
            led_hold <= 26'b0;
            pwm <= 676'b0;
        end

        else begin

		  for (i=0; i<26; i=i+1) begin
            if (avalon_slave_writedata[i] == 1) begin
                    cnt[(i*26) +: 25] <= 26'h3FFFFFF;
            end
			end


            for (i = 0; i < 26; i = i + 1) begin
                if (cnt[(i*26) +: 25] > 0) begin
                    cnt[(i*26) +: 25] <= cnt[(i*26) +: 25] - 1;
                end
            end

 

            for (i = 0; i < 26; i = i + 1) begin
                if (pwm[(i*26) +: 25] == 26'h3FFFFFF) begin
                    pwm[(i*26) +: 25] <= 26'b0;
                end
                else begin
                    pwm[(i*26) +: 25] <= pwm[(i*26) +: 25] + 1;
                end

            end


            for (i = 0; i < 26; i = i + 1) begin
                if (cnt[((i*26)+14) +: 11] > pwm[(i*26) +: 11]) begin
                    led_hold[i] <= 1'b1;
                end
                else begin
                    led_hold[i] <= 1'b0;
                end
            end
        end
    end

    assign leds = led_hold;


endmodule