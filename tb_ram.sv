`timescale 1ns / 1ps

module tb_ram;

  // Entradas
  reg [10:0] address;
  reg clock;
  reg [15:0] data;
  reg wren;

  // Salida
  wire [15:0] q;

  // Instancia del módulo RAM
  ram uut (
    .address(address),
    .clock(clock),
    .data(data),
    .wren(wren),
    .q(q)
  );

  // Generador de reloj
  initial begin
    clock = 0;
    forever #5 clock = ~clock; // Periodo de 10 ns
  end

  // Proceso de prueba
  initial begin
    // Inicialización
    wren = 0;
    address = 0;
    data = 0;

    // Espera inicial
    #10;

    // Escritura en la dirección 0x001
    address = 11'h001;
    data = 16'hABCD;
    wren = 1;
    #10;

    // Escritura en la dirección 0x002
    address = 11'h002;
    data = 16'h1234;
    #10;

    // Escritura en la dirección 0x003
    address = 11'h003;
    data = 16'h5678;
    #10;

    // Terminar escritura
    wren = 0;

    // Leer dirección 0x001
    address = 11'h001;
    #10;
    $display("Direccion 0x001, Dato leido: %h (Esperado: ABCD)", q);

    // Leer dirección 0x002
    address = 11'h002;
    #10;
    $display("Direccion 0x002, Dato leido: %h (Esperado: 1234)", q);

    // Leer dirección 0x003
    address = 11'h003;
    #10;
    $display("Direccion 0x003, Dato leido: %h (Esperado: 5678)", q);

    // Finalizar simulación
    $stop;
  end

endmodule
