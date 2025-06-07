module sdram_interface #(
    parameter ADDR_WIDTH = 24,       
    parameter DATA_WIDTH = 16
)(
    input  logic clk,
    input  logic rst,

    
    input  logic start_write,
    input  logic start_read,
    input  logic [ADDR_WIDTH-1:0] addr,
    input  logic [DATA_WIDTH-1:0] data_in,
    output logic [DATA_WIDTH-1:0] data_out,
    output logic done,

    // para conectar al SDRAM Controller
    output logic [ADDR_WIDTH-1:0] avm_address,
    output logic avm_read,
    output logic avm_write,
    output logic [DATA_WIDTH-1:0] avm_writedata,
    input  logic [DATA_WIDTH-1:0] avm_readdata,
    input  logic avm_waitrequest,
    input  logic avm_readdatavalid


);

    
    typedef enum logic [1:0] {
        IDLE,
        WRITE,
        READ,
        DONE
    } state_t;

    state_t state, next_state;

   
    always_ff @(posedge clk or posedge rst) begin
        if (rst)
            state <= IDLE;
        else
            state <= next_state;
    end

   
    always_comb begin
        next_state = state;
        case (state)
            IDLE: begin
                if (start_write)
                    next_state = WRITE;
                else if (start_read)
                    next_state = READ;
            end

            WRITE: begin
                if (~avm_waitrequest)
                    next_state = DONE;
            end

            READ: begin
                if (avm_readdatavalid)
                    next_state = DONE;
            end

            DONE: begin
                next_state = IDLE;
            end
        endcase
    end

    
    assign avm_address    = addr;
    assign avm_writedata  = data_in;
    assign avm_write = (state == WRITE);
    assign avm_read  = (state == READ);

    
    always_ff @(posedge clk or posedge rst) begin
        if (rst)
            data_out <= 0;
        else if (avm_readdatavalid)
            data_out <= avm_readdata;
    end
endmodule
