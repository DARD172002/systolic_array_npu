module PE #(
    parameter DATA_WIDTH = 16
)(
    input  logic clk,
    input  logic rst,

    input  logic signed [DATA_WIDTH-1:0] a_in,
    input  logic signed [DATA_WIDTH-1:0] b_in,
    input  logic valid_in,

    output logic signed [DATA_WIDTH-1:0] a_out,
    output logic signed [DATA_WIDTH-1:0] b_out,

    output logic signed [DATA_WIDTH-1:0] result,
    output logic valid_out
);

    logic signed [DATA_WIDTH-1:0] a_reg, b_reg;
    logic signed [(2*DATA_WIDTH)-1:0] acc;

    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            a_reg     <= 0;
            b_reg     <= 0;
            acc       <= 0;
            valid_out <= 0;
        end else begin
            if (valid_in) begin
                a_reg     <= a_in;
                b_reg     <= b_in;
                acc       <= acc + (a_in * b_in);
                valid_out <= 1;
            end else begin
                valid_out <= 0;
            end
        end
    end

    
    assign a_out = a_reg;
    assign b_out = b_reg;

    // Parte del relu 
    wire signed [DATA_WIDTH-1:0] acc_trunc = acc[DATA_WIDTH-1:0];
    assign result = (acc_trunc > 0) ? acc_trunc : 0;

endmodule

