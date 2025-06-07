module systolic_array_4x4 #(
    parameter DATA_WIDTH = 16,
    parameter N = 2
)(
    input  logic clk,
    input  logic rst,
    
    input  logic signed [DATA_WIDTH-1:0] A_in[N],
    input  logic signed [DATA_WIDTH-1:0] B_in[N],
    input  logic valid_in,

    output logic signed [DATA_WIDTH-1:0] result[N][N],
    output logic valid_out[N][N]
);

    logic signed [DATA_WIDTH-1:0] a_bus [N][N];
    logic signed [DATA_WIDTH-1:0] b_bus [N][N];
    logic signed [DATA_WIDTH-1:0] res_bus [N][N];
    logic                        valid_bus[N][N];

    genvar i, j;
    generate
        for (i = 0; i < N; i++) begin: rows
            for (j = 0; j < N; j++) begin: cols

                logic signed [DATA_WIDTH-1:0] a_in, b_in;
                logic signed [DATA_WIDTH-1:0] a_out, b_out;
                logic signed [DATA_WIDTH-1:0] res;
                logic                        v_in, v_out;

                if (j == 0) begin
                    assign a_in = A_in[i];
                end else begin
                    assign a_in = a_bus[i][j-1];
                end

                if (i == 0) begin
                    assign b_in = B_in[j];
                end else begin
                    assign b_in = b_bus[i-1][j];
                end

                if (i == 0 && j == 0) begin
                    assign v_in = valid_in;
                end else if (i == 0) begin
                    assign v_in = valid_bus[i][j-1]; // izquierda
                end else if (j == 0) begin
                    assign v_in = valid_bus[i-1][j]; // arriba
                end else begin
                    assign v_in = valid_bus[i-1][j-1]; // diagonal
                end

                PE #(.DATA_WIDTH(DATA_WIDTH)) pe_inst (
                    .clk(clk),
                    .rst(rst),
                    .a_in(a_in),
                    .b_in(b_in),
                    .valid_in(v_in),
                    .a_out(a_out),
                    .b_out(b_out),
                    .result(res),
                    .valid_out(v_out)
                );

                assign a_bus[i][j] = a_out;
                assign b_bus[i][j] = b_out;
                assign res_bus[i][j] = res;
                assign valid_bus[i][j] = v_out;

                assign result[i][j] = res_bus[i][j];
                assign valid_out[i][j] = valid_bus[i][j];

            end
        end
    endgenerate

endmodule
