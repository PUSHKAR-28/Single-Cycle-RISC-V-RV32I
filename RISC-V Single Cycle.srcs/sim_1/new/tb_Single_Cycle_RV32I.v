`timescale 1ns / 1ps

module tb_Single_Cycle_RV32I;

    reg clk;
    reg reset;

    Single_Cycle_CPU_Top dut (
        .clk(clk),
        .reset(reset)
    );

    initial begin
        clk = 0;
        forever #10 clk = ~clk;
    end

    // ----------------------------------------------------------------
    // Test number -> instruction name lookup
    // ----------------------------------------------------------------
    function [8*40:1] test_name;
        input integer n;
        begin
            case (n)
                1:  test_name = "ADDI positive                   ";
                2:  test_name = "ADDI negative                   ";
                3:  test_name = "ADDI zero                       ";
                4:  test_name = "SLTI: 3 < 10                    ";
                5:  test_name = "SLTI: -1 < 0 (signed)           ";
                6:  test_name = "SLTIU: 0xFFFFFFFF < 1 (unsigned)";
                7:  test_name = "XORI: 0xFF ^ 0x0F               ";
                8:  test_name = "ORI:  0xA0 | 0x0F               ";
                9:  test_name = "ANDI: 0xFF & 0x0F               ";
                10: test_name = "SLLI: 1 << 4                    ";
                11: test_name = "SRLI: 16 >> 2                   ";
                12: test_name = "SRAI: -16 >> 2                  ";
                13: test_name = "ADD:  15 + 27                   ";
                14: test_name = "SUB:  27 - 15                   ";
                15: test_name = "SLL:  3 << 3                    ";
                16: test_name = "SRL:  24 >> 3                   ";
                17: test_name = "SRA:  -24 >> 3                  ";
                18: test_name = "SLT:  5 < 10 (signed)           ";
                19: test_name = "SLT:  -1 < 1 (signed)           ";
                20: test_name = "SLTU: 0xFFFFFFFF < 1 (unsigned) ";
                21: test_name = "XOR:  0xFF ^ 0x0F               ";
                22: test_name = "OR:   0xFF | 0x0F               ";
                23: test_name = "AND:  0xFF & 0x0F               ";
                24: test_name = "LUI                             ";
                25: test_name = "AUIPC                           ";
                26: test_name = "SW + LW roundtrip               ";
                27: test_name = "SB + LB (signed byte)           ";
                28: test_name = "LBU (unsigned byte)             ";
                29: test_name = "SH + LH (signed halfword)       ";
                30: test_name = "LHU (unsigned halfword)         ";
                31: test_name = "BEQ taken                       ";
                32: test_name = "BNE taken                       ";
                33: test_name = "BLT taken (signed)              ";
                34: test_name = "BGE taken (signed)              ";
                35: test_name = "BLTU taken (unsigned)           ";
                36: test_name = "BGEU taken (unsigned)           ";
                37: test_name = "JAL + JALR return               ";
                38: test_name = "JALR jump                       ";
                default: test_name = "UNKNOWN                         ";
            endcase
        end
    endfunction

    initial begin
        $readmemh("C:/Users/PUSHKAR/Desktop/PROJECTS/RISC-V Single Cycle/RV32_test/IMEM.hex", dut.IMEM_inst.IMEM);
        $readmemh("C:/Users/PUSHKAR/Desktop/PROJECTS/RISC-V Single Cycle/RV32_test/DMEM.hex",        dut.DMEM_inst.mem);

        reset = 1;
        #25;
        reset = 0;

        // Run long enough for all 38 tests (206 instructions + margin)
        // At 20ns/cycle: 206 instr * 20ns = ~4120ns, give 2x margin
        #500000;
        $finish;
    end

    // ----------------------------------------------------------------
    // Monitor: watch pass/fail addresses every cycle
    // Pass addr = 256 (word index 64 in DMEM)
    // Fail addr = 260 (word index 65 in DMEM)
    // ----------------------------------------------------------------
    integer printed = 0;

    always @(negedge clk) begin
        if (!reset && !printed) begin

            // PASS: DEADBEEF written to DMEM[64] (byte addr 256)
            if (dut.DMEM_inst.mem[64] === 32'hDEADBEEF) begin
                $display("============================================================");
                $display("  ALL 38 TESTS PASSED");
                $display("  PC = 0x%h  Time = %0t ns", dut.PC_out, $time/1000);
                $display("============================================================");
                printed = 1;
                #40;
                $finish;
            end

            // FAIL: test number written to DMEM[65] (byte addr 260)
            if (dut.DMEM_inst.mem[65] !== 32'h0 && dut.DMEM_inst.mem[65] !== 32'hx) begin
                $display("============================================================");
                $display("  FAILED at test %0d: %s", dut.DMEM_inst.mem[65], test_name(dut.DMEM_inst.mem[65]));
                $display("  PC = 0x%h  Time = %0t ns", dut.PC_out, $time/1000);
                $display("  s0 (test counter) = %0d", dut.Reg_File_inst.Registers[8]);
                $display("  t0 (expected)     = 0x%h (%0d)", dut.Reg_File_inst.Registers[5], $signed(dut.Reg_File_inst.Registers[5]));
                $display("  t1 (actual)       = 0x%h (%0d)", dut.Reg_File_inst.Registers[6], $signed(dut.Reg_File_inst.Registers[6]));
                $display("============================================================");
                printed = 1;
                #40;
                $finish;
            end
        end
    end

    // Timeout safety net
    initial begin
        #500000;
        if (!printed) begin
            $display("============================================================");
            $display("  TIMEOUT - CPU never reached PASS or FAIL");
            $display("  Final PC = 0x%h", dut.PC_out);
            $display("  DMEM[64] (pass addr) = 0x%h", dut.DMEM_inst.mem[64]);
            $display("  DMEM[65] (fail addr) = 0x%h", dut.DMEM_inst.mem[65]);
            $display("  s0 (test counter)    = %0d", dut.Reg_File_inst.Registers[8]);
            $display("============================================================");
            $finish;
        end
    end

endmodule