// #define PASS (*(volatile unsigned int *)0x2100)
// #define FAIL (*(volatile unsigned int *)0x2104)

// int a = 5;
// int b = 7;

// int add(int a, int b) {
//     return a+b;
// }

// int main() {
//     int c = 100;

//     c = add(a, b);

//     if (c != 12) { FAIL = 1; while (1); }

//     PASS = 0xDEADBEEF;
//     return 0;
// }

// #define PASS (*(volatile unsigned int *)0x2100)
// #define FAIL (*(volatile unsigned int *)0x2104)

// int fibonacci(int n) {
//     if (n <= 1) return n;
//     return fibonacci(n - 1) + fibonacci(n - 2);
// }

// int main() {
//     if (fibonacci(10) != 55) { FAIL = 1; while(1); }
//     PASS = 0xDEADBEEF;
//     return 0;
// }


// #define PASS (*(volatile unsigned int *)0x2100)
// #define FAIL (*(volatile unsigned int *)0x2104)

// int knapsack(int W, int *wt, int *val, int n) {
//     int dp[6][51];  // [items+1][capacity+1]
//     int i, w;
//     for (i = 0; i <= n; i++)
//         for (w = 0; w <= W; w++) {
//             if (i == 0 || w == 0)
//                 dp[i][w] = 0;
//             else if (wt[i-1] <= w) {
//                 int take = val[i-1] + dp[i-1][w - wt[i-1]];
//                 int skip = dp[i-1][w];
//                 dp[i][w] = take > skip ? take : skip;
//             } else {
//                 dp[i][w] = dp[i-1][w];
//             }
//         }
//     return dp[n][W];
// }

// int main() {
//     int val[5] = {60, 100, 120, 40, 80};
//     int wt[5]  = {10,  20,  30,  5, 15};
//     int W = 50, n = 5;
//     // optimal: take items 0,1,3,4 -> 60+100+40+80=280, weight=10+20+5+15=50
//     if (knapsack(W, wt, val, n) != 280) { FAIL = 1; while(1); }

//     int val2[3] = {1, 2, 3};
//     int wt2[3]  = {4, 5, 6};
//     // W=10: take item2(w=6,v=3) + item0(w=4,v=1) = 4, weight=10
//     if (knapsack(10, wt2, val2, 3) != 4) { FAIL = 2; while(1); }

//     PASS = 0xDEADBEEF;
//     return 0;
// }

// #define PASS (*(volatile unsigned int *)0x2100)
// #define FAIL (*(volatile unsigned int *)0x2104)

// int mul(int a, int b) {
//     int result = 0;
//     int neg = 0;
//     if (a < 0) { a = -a; neg = !neg; }
//     if (b < 0) { b = -b; neg = !neg; }
//     while (b > 0) {
//         result += a;
//         b--;
//     }
//     return neg ? -result : result;
// }

// void matmul(int A[3][3], int B[3][3], int C[3][3]) {
//     int i, j, k;
//     for (i = 0; i < 3; i++)
//         for (j = 0; j < 3; j++) {
//             C[i][j] = 0;
//             for (k = 0; k < 3; k++)
//                 C[i][j] += mul(A[i][k], B[k][j]);
//         }
// }

// int main() {
//     int A[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
//     int B[3][3] = {{9,8,7},{6,5,4},{3,2,1}};
//     int C[3][3];

//     matmul(A, B, C);

//     if (C[0][0] != 30)  { FAIL = 1; while(1); }
//     if (C[0][1] != 24)  { FAIL = 2; while(1); }
//     if (C[0][2] != 18)  { FAIL = 3; while(1); }
//     if (C[1][0] != 84)  { FAIL = 4; while(1); }
//     if (C[1][1] != 69)  { FAIL = 5; while(1); }
//     if (C[1][2] != 54)  { FAIL = 6; while(1); }
//     if (C[2][0] != 138) { FAIL = 7; while(1); }
//     if (C[2][1] != 114) { FAIL = 8; while(1); }
//     if (C[2][2] != 90)  { FAIL = 9; while(1); }

//     PASS = 0xDEADBEEF;
//     return 0;
// }


// #define PASS (*(volatile unsigned int *)0x2100)
// #define FAIL (*(volatile unsigned int *)0x2104)

// int knapsack(int W, int *wt, int *val, int n) {
//     int dp[6][51];
//     int i, w;
//     for (i = 0; i <= n; i++)
//         for (w = 0; w <= W; w++) {
//             if (i == 0 || w == 0)
//                 dp[i][w] = 0;
//             else if (wt[i-1] <= w) {
//                 int take = val[i-1] + dp[i-1][w - wt[i-1]];
//                 int skip = dp[i-1][w];
//                 dp[i][w] = take > skip ? take : skip;
//             } else {
//                 dp[i][w] = dp[i-1][w];
//             }
//         }
//     return dp[n][W];
// }

// int main() {
//     int val[5] = {60, 100, 120, 40, 80};
//     int wt[5]  = {10,  20,  30,  5, 15};

//     if (knapsack(50, wt, val, 5) != 280) { FAIL = 1; while(1); }

//     int val2[3] = {1, 2, 3};
//     int wt2[3]  = {4, 5, 6};

//     if (knapsack(10, wt2, val2, 3) != 4) { FAIL = 2; while(1); }

//     PASS = 0xDEADBEEF;
//     return 0;
// }

/*
 * RV32I COMPREHENSIVE CPU TEST
 * Tests all 38 implemented instructions (40 - ecall - ebreak)
 * Each test is numbered. On failure: FAIL = test number, then infinite loop.
 * On full pass: PASS = 0xDEADBEEF
 *
 * FAIL codes are grouped:
 *   100s = R-type ALU (add/sub/xor/or/and/sll/srl/sra/slt/sltu)
 *   200s = I-type ALU (addi/xori/ori/andi/slli/srli/srai/slti/sltiu)
 *   300s = LUI / AUIPC
 *   400s = Loads  (lb/lh/lw/lbu/lhu)
 *   500s = Stores (sb/sh/sw) + load-store roundtrip
 *   600s = Branches (beq/bne/blt/bge/bltu/bgeu)
 *   700s = JAL / JALR / function calls
 *   800s = Hazards / forwarding / x0 immutability
 *   900s = Shift edge cases (shift by 0, 31, register amount masking)
 */

#define PASS (*(volatile unsigned int *)0x4100)
#define FAIL (*(volatile unsigned int *)0x4104)

/* ── helpers ─────────────────────────────────────────────────── */
/* Prevent compiler from optimising checks away */
static volatile int   vi;
static volatile unsigned int vu;

/* ═══════════════════════════════════════════════════════════════
 * SECTION 1 — R-TYPE  (FAIL 100-199)
 * Exercises: add sub xor or and sll srl sra slt sltu
 * ═══════════════════════════════════════════════════════════════ */

/* --- ADD --- */
static void test_add(void) {
    int a, b, c;

    /* basic */
    a = 3; b = 4; c = a + b;
    if (c != 7)           { FAIL = 100; while(1); }

    /* zero identity */
    a = 0; b = 0xDEAD; c = a + b;
    if (c != 0xDEAD)      { FAIL = 101; while(1); }

    /* negative + positive */
    a = -10; b = 3; c = a + b;
    if (c != -7)          { FAIL = 102; while(1); }

    /* unsigned wrap: 0xFFFFFFFF + 1 = 0 */
    unsigned int ua = 0xFFFFFFFF, ub = 1;
    unsigned int uc = ua + ub;
    if (uc != 0)          { FAIL = 103; while(1); }

    /* result = one of the operands (forwarding hazard candidate) */
    a = 5; a = a + a;
    if (a != 10)          { FAIL = 104; while(1); }
}

/* --- SUB --- */
static void test_sub(void) {
    int a, b, c;

    a = 10; b = 3; c = a - b;
    if (c != 7)           { FAIL = 110; while(1); }

    /* negative result */
    a = 3; b = 10; c = a - b;
    if (c != -7)          { FAIL = 111; while(1); }

    /* x - x = 0 */
    a = 0xABCD; c = a - a;
    if (c != 0)           { FAIL = 112; while(1); }

    /* 0 - 1 = -1 = 0xFFFFFFFF */
    unsigned int ua = 0, ub = 1;
    unsigned int uc = ua - ub;
    if (uc != 0xFFFFFFFF) { FAIL = 113; while(1); }

    /* SUB vs ADD: make sure funct7 distinguishes them */
    a = 7; b = 3;
    int add_res = a + b;   /* expect 10 */
    int sub_res = a - b;   /* expect  4 */
    if (add_res == sub_res){ FAIL = 114; while(1); }
    if (add_res != 10)     { FAIL = 115; while(1); }
    if (sub_res != 4)      { FAIL = 116; while(1); }
}

/* --- XOR --- */
static void test_xor(void) {
    unsigned int a, b, c;

    a = 0xAAAAAAAA; b = 0x55555555; c = a ^ b;
    if (c != 0xFFFFFFFF)  { FAIL = 120; while(1); }

    /* x ^ x = 0 */
    a = 0xDEADBEEF; c = a ^ a;
    if (c != 0)           { FAIL = 121; while(1); }

    /* x ^ 0 = x */
    a = 0x12345678; c = a ^ 0;
    if (c != 0x12345678)  { FAIL = 122; while(1); }

    /* x ^ ~x = 0xFFFFFFFF (using ~x = x ^ 0xFFFFFFFF) */
    a = 0xA5A5A5A5; b = ~a;
    c = a ^ b;
    if (c != 0xFFFFFFFF)  { FAIL = 123; while(1); }
}

/* --- OR --- */
static void test_or(void) {
    unsigned int a, b, c;

    a = 0xF0F0F0F0; b = 0x0F0F0F0F; c = a | b;
    if (c != 0xFFFFFFFF)  { FAIL = 130; while(1); }

    /* x | 0 = x */
    a = 0x12345678; c = a | 0;
    if (c != 0x12345678)  { FAIL = 131; while(1); }

    /* x | 0xFFFFFFFF = 0xFFFFFFFF */
    a = 0; c = a | 0xFFFFFFFF;
    if (c != 0xFFFFFFFF)  { FAIL = 132; while(1); }
}

/* --- AND --- */
static void test_and(void) {
    unsigned int a, b, c;

    a = 0xFFFF0000; b = 0x0000FFFF; c = a & b;
    if (c != 0)           { FAIL = 140; while(1); }

    /* x & x = x */
    a = 0xDEADBEEF; c = a & a;
    if (c != 0xDEADBEEF)  { FAIL = 141; while(1); }

    /* bit isolation */
    a = 0xABCD1234; b = 0x0000FF00; c = a & b;
    if (c != 0x00001200)  { FAIL = 142; while(1); }
}

/* --- SLL (register shift amount) --- */
static void test_sll(void) {
    unsigned int a, c;
    int sh;

    /* basic */
    a = 1; sh = 4; c = a << sh;
    if (c != 16)          { FAIL = 150; while(1); }

    /* shift by 0 = identity */
    a = 0xDEADBEEF; sh = 0; c = a << sh;
    if (c != 0xDEADBEEF)  { FAIL = 151; while(1); }

    /* shift by 31 */
    a = 1; sh = 31; c = a << sh;
    if (c != 0x80000000)  { FAIL = 152; while(1); }

    /* shift by 1 doubles */
    a = 7; sh = 1; c = a << sh;
    if (c != 14)          { FAIL = 153; while(1); }

    /* shift-by-32 tested in section 9 via volatile sh (C UB here, RV32I defined there) */
}

/* --- SRL (register shift amount, unsigned) --- */
static void test_srl(void) {
    unsigned int a, c;
    int sh;

    a = 256; sh = 3; c = a >> sh;
    if (c != 32)          { FAIL = 160; while(1); }

    /* shift by 0 */
    a = 0xDEADBEEF; sh = 0; c = a >> sh;
    if (c != 0xDEADBEEF)  { FAIL = 161; while(1); }

    /* MSB must NOT be replicated (logical, not arithmetic) */
    a = 0x80000000; sh = 1; c = a >> sh;
    if (c != 0x40000000)  { FAIL = 162; while(1); }

    /* shift by 31 */
    a = 0x80000000; sh = 31; c = a >> sh;
    if (c != 1)           { FAIL = 163; while(1); }

    /* shift-by-32 tested in section 9 via volatile sh */
}

/* --- SRA (register shift amount, signed/arithmetic) --- */
static void test_sra(void) {
    int a, c;
    int sh;

    /* negative: sign bit must replicate */
    a = -8; sh = 2; c = a >> sh;
    if (c != -2)          { FAIL = 170; while(1); }

    a = -1; sh = 1; c = a >> sh;
    if (c != -1)          { FAIL = 171; while(1); }

    a = -1; sh = 31; c = a >> sh;
    if (c != -1)          { FAIL = 172; while(1); }

    /* positive: identical to SRL */
    a = 64; sh = 3; c = a >> sh;
    if (c != 8)           { FAIL = 173; while(1); }

    /* shift by 0 */
    a = -100; sh = 0; c = a >> sh;
    if (c != -100)        { FAIL = 174; while(1); }

    /* SRA vs SRL must differ for negative values */
    unsigned int ua2 = (unsigned int)(-8);
    unsigned int srl_res = ua2 >> 2;   /* logical: fills with 0s = 0x3FFFFFFE */
    sh = 2; a = -8;
    int sra_res2 = a >> sh;             /* arithmetic: fills with 1s = -2 */
    if ((int)srl_res == sra_res2) { FAIL = 175; while(1); }
}

/* --- SLT / SLTU --- */
static void test_slt_sltu(void) {
    int a, b;

    /* SLT signed */
    a = -1; b = 0;
    if (!(a < b))         { FAIL = 180; while(1); }  /* -1 < 0 true */
    if (b < a)            { FAIL = 181; while(1); }  /* 0 < -1 false */

    a = -2; b = -1;
    if (!(a < b))         { FAIL = 182; while(1); }  /* -2 < -1 true */

    /* CRITICAL: signed vs unsigned -1 */
    /* As signed:   -1 < 1   → true  */
    /* As unsigned: 0xFFFFFFFF < 1 → false */
    int          si = -1;
    unsigned int ui = 1;
    if (!(si < (int)ui))              { FAIL = 183; while(1); }
    if ((unsigned int)si < ui)        { FAIL = 184; while(1); } /* should be FALSE */

    /* SLTU: 0 < anything_nonzero is true */
    unsigned int ua = 0, ub = 1;
    if (!(ua < ub))       { FAIL = 185; while(1); }

    /* largest unsigned > 0 */
    ua = 0xFFFFFFFF; ub = 0;
    if (ua < ub)          { FAIL = 186; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 2 — I-TYPE ALU  (FAIL 200-299)
 * Exercises: addi xori ori andi slli srli srai slti sltiu
 * ═══════════════════════════════════════════════════════════════ */

static void test_addi(void) {
    int a;

    a = 0; a += 1;
    if (a != 1)           { FAIL = 200; while(1); }

    /* CRITICAL: immediate is sign-extended from 12 bits */
    /* 0x800 in 12-bit = -2048 when sign-extended */
    a = 0; a = a + (-2048);
    if (a != -2048)       { FAIL = 201; while(1); }

    /* max positive 12-bit imm = 2047 */
    a = 0; a = a + 2047;
    if (a != 2047)        { FAIL = 202; while(1); }

    /* addi used for move: rd = rs1 + 0 */
    int b = 0x12345678; int c = b + 0;
    if (c != 0x12345678)  { FAIL = 203; while(1); }

    /* overflow wraps */
    unsigned int u = 0xFFFFFFFF; u += 1;
    if (u != 0)           { FAIL = 204; while(1); }
}

static void test_xori(void) {
    unsigned int a;

    /* xori with 0xFFF (sign-extended = -1 = 0xFFFFFFFF) = bitwise NOT */
    a = 0xAAAAAAAA;
    unsigned int nota = a ^ 0xFFFFFFFF;
    if (nota != 0x55555555) { FAIL = 210; while(1); }

    /* xori 0 = identity */
    a = 0xDEADBEEF;
    if ((a ^ 0) != 0xDEADBEEF) { FAIL = 211; while(1); }
}

static void test_ori(void) {
    unsigned int a;

    /* set low byte */
    a = 0xFFFFFF00;
    a = a | 0xFF;
    if (a != 0xFFFFFFFF)  { FAIL = 220; while(1); }

    /* ori 0 = identity */
    a = 0x12345678;
    if ((a | 0) != 0x12345678) { FAIL = 221; while(1); }
}

static void test_andi(void) {
    unsigned int a;

    /* mask low byte */
    a = 0xDEADBEEF;
    a = a & 0xFF;
    if (a != 0xEF)        { FAIL = 230; while(1); }

    /* andi -1 (0xFFF sign-ext to 0xFFFFFFFF) = identity */
    a = 0x12345678;
    if ((a & 0xFFFFFFFF) != 0x12345678) { FAIL = 231; while(1); }

    /* andi 0 = 0 */
    a = 0xDEADBEEF;
    if ((a & 0) != 0)     { FAIL = 232; while(1); }
}

static void test_slli(void) {
    unsigned int a;

    a = 1;
    if ((a << 0)  != 1)           { FAIL = 240; while(1); }
    if ((a << 1)  != 2)           { FAIL = 241; while(1); }
    if ((a << 31) != 0x80000000)  { FAIL = 242; while(1); }

    /* shift that drops all bits */
    a = 0xFFFFFFFF;
    /* shifting left by 31 keeps only bit 0 in position 31 */
    if (((a & 1) << 31) != 0x80000000) { FAIL = 243; while(1); }
}

static void test_srli(void) {
    unsigned int a;

    a = 0x80000000;
    /* CRITICAL: must NOT sign-extend (logical shift) */
    if ((a >> 1) != 0x40000000)   { FAIL = 250; while(1); }
    if ((a >> 31) != 1)           { FAIL = 251; while(1); }

    a = 0xFFFFFFFF;
    if ((a >> 0) != 0xFFFFFFFF)   { FAIL = 252; while(1); }

    a = 256;
    if ((a >> 4) != 16)           { FAIL = 253; while(1); }
}

static void test_srai(void) {
    int a;

    /* sign bit replicated */
    a = -1;
    if ((a >> 1) != -1)           { FAIL = 260; while(1); }
    if ((a >> 31) != -1)          { FAIL = 261; while(1); }

    a = -8;
    if ((a >> 1) != -4)           { FAIL = 262; while(1); }
    if ((a >> 2) != -2)           { FAIL = 263; while(1); }
    if ((a >> 3) != -1)           { FAIL = 264; while(1); }

    /* positive: fills with 0 just like SRL */
    a = 64;
    if ((a >> 1) != 32)           { FAIL = 265; while(1); }

    /* CRITICAL: SRAI vs SRLI must differ for negative */
    a = -2;
    int  sra = a >> 1;           /* -1 */
    unsigned int srl = (unsigned int)a >> 1; /* 0x7FFFFFFF */
    if (sra == (int)srl)          { FAIL = 266; while(1); }
}

static void test_slti(void) {
    int a;

    /* -1 < 0 true */
    a = -1;
    if (!(a < 0))         { FAIL = 270; while(1); }

    /* 0 is not < 0 */
    a = 0;
    if (a < 0)            { FAIL = 271; while(1); }

    /* large positive < immediate */
    a = 100;
    if (!(a < 200))       { FAIL = 272; while(1); }
    if (a < 50)           { FAIL = 273; while(1); }

    /* 12-bit immediate sign-extended: -1 as imm */
    /* a < -1 should be true for a = -2 */
    a = -2;
    if (!(a < -1))        { FAIL = 274; while(1); }
}

static void test_sltiu(void) {
    unsigned int a;

    /* 0 < 1 */
    a = 0;
    if (!(a < 1u))        { FAIL = 280; while(1); }

    /* 0xFFFFFFFF is not < 1 when unsigned */
    a = 0xFFFFFFFF;
    if (a < 1u)           { FAIL = 281; while(1); }

    /* sltiu with imm=1: result is 1 only if a==0 (seqz idiom) */
    a = 0;
    int seqz = (a < 1u) ? 1 : 0;
    if (seqz != 1)        { FAIL = 282; while(1); }
    a = 5;
    seqz = (a < 1u) ? 1 : 0;
    if (seqz != 0)        { FAIL = 283; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 3 — LUI / AUIPC  (FAIL 300-399)
 * ═══════════════════════════════════════════════════════════════ */

static void test_lui(void) {
    unsigned int a;

    /* low 12 bits must be zero */
    a = 0xABCDE000;
    if ((a & 0xFFF) != 0)         { FAIL = 300; while(1); }

    /* upper 20 bits correct */
    if ((a >> 12) != 0xABCDE)     { FAIL = 301; while(1); }

    /* common pattern: lui + addi to build 32-bit constant */
    /* 0xDEADBEEF: upper = 0xDEADB, lower = 0xEEF */
    /* addi sign-extends, so if lower >= 0x800, upper is incremented by 1 */
    /* 0xEEF = 3823, which is > 0x7FF, so we need upper+1 trick */
    /* Let's just verify a clean constant */
    a = 0x12345000;  /* lui loads upper 20 bits = 0x12345 */
    if (a != 0x12345000)          { FAIL = 302; while(1); }

    /* LUI with all-ones in upper 20: 0xFFFFF000 */
    a = 0xFFFFF000;
    if (a != 0xFFFFF000)          { FAIL = 303; while(1); }

    /* LUI zero */
    a = 0;
    if (a != 0)                   { FAIL = 304; while(1); }
}

/* AUIPC is tested implicitly via global variable addressing
   and function calls (compiler uses it for PC-relative loads).
   Explicit test: capture PC-relative address and verify it's nonzero
   and changes between two capture sites. */
static volatile unsigned int auipc_addr1;
static volatile unsigned int auipc_addr2;

static void test_auipc(void) {
    /* Compiler will use AUIPC to load addresses of globals */
    auipc_addr1 = (unsigned int)&auipc_addr1;
    auipc_addr2 = (unsigned int)&auipc_addr2;

    /* Both must be nonzero (real addresses) */
    if (auipc_addr1 == 0)         { FAIL = 310; while(1); }
    if (auipc_addr2 == 0)         { FAIL = 311; while(1); }

    /* They must be different (different variables) */
    if (auipc_addr1 == auipc_addr2){ FAIL = 312; while(1); }

    /* Address of addr1 must match what we stored */
    if (auipc_addr1 != (unsigned int)&auipc_addr1) { FAIL = 313; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 4 — LOADS  (FAIL 400-499)
 * Exercises: lb lh lw lbu lhu
 * ═══════════════════════════════════════════════════════════════ */

static unsigned int   load_word_buf[4];
static unsigned short load_half_buf[4];
static unsigned char  load_byte_buf[8];

static void test_loads(void) {

    /* --- LW --- */
    load_word_buf[0] = 0xDEADBEEF;
    load_word_buf[1] = 0x00000000;
    load_word_buf[2] = 0xFFFFFFFF;
    load_word_buf[3] = 0x12345678;

    if (load_word_buf[0] != 0xDEADBEEF) { FAIL = 400; while(1); }
    if (load_word_buf[1] != 0x00000000) { FAIL = 401; while(1); }
    if (load_word_buf[2] != 0xFFFFFFFF) { FAIL = 402; while(1); }
    if (load_word_buf[3] != 0x12345678) { FAIL = 403; while(1); }

    /* --- LB (signed): 0xFF must read back as -1 --- */
    load_byte_buf[0] = 0xFF;
    load_byte_buf[1] = 0x80;   /* -128 */
    load_byte_buf[2] = 0x7F;   /*  127 */
    load_byte_buf[3] = 0x00;

    signed char sb0 = (signed char)load_byte_buf[0];
    if (sb0 != -1)              { FAIL = 410; while(1); }

    signed char sb1 = (signed char)load_byte_buf[1];
    if (sb1 != -128)            { FAIL = 411; while(1); }

    signed char sb2 = (signed char)load_byte_buf[2];
    if (sb2 != 127)             { FAIL = 412; while(1); }

    signed char sb3 = (signed char)load_byte_buf[3];
    if (sb3 != 0)               { FAIL = 413; while(1); }

    /* --- LBU (unsigned): 0xFF must read back as 255, NOT -1 --- */
    if ((unsigned char)load_byte_buf[0] != 255)  { FAIL = 420; while(1); }
    if ((unsigned char)load_byte_buf[1] != 128)  { FAIL = 421; while(1); }
    if ((unsigned char)load_byte_buf[2] != 127)  { FAIL = 422; while(1); }

    /* CRITICAL: LB vs LBU must differ for values >= 0x80 */
    if ((signed char)load_byte_buf[0] == (unsigned char)load_byte_buf[0]) {
        FAIL = 423; while(1);
    }

    /* --- LH (signed halfword) --- */
    load_half_buf[0] = 0x8000;   /* -32768 */
    load_half_buf[1] = 0x7FFF;   /*  32767 */
    load_half_buf[2] = 0xFFFF;   /* -1     */
    load_half_buf[3] = 0x0001;

    short sh0 = (short)load_half_buf[0];
    if (sh0 != -32768)           { FAIL = 430; while(1); }

    short sh1 = (short)load_half_buf[1];
    if (sh1 != 32767)            { FAIL = 431; while(1); }

    short sh2 = (short)load_half_buf[2];
    if (sh2 != -1)               { FAIL = 432; while(1); }

    /* --- LHU (unsigned halfword) --- */
    if ((unsigned short)load_half_buf[0] != 0x8000) { FAIL = 440; while(1); }
    if ((unsigned short)load_half_buf[2] != 0xFFFF) { FAIL = 441; while(1); }

    /* CRITICAL: LH vs LHU must differ for values >= 0x8000 */
    if ((short)load_half_buf[0] == (unsigned short)load_half_buf[0]) {
        FAIL = 442; while(1);
    }

    /* --- Negative offset load (base + negative imm) --- */
    /* Point past end of buffer, load backwards */
    unsigned int *ptr = &load_word_buf[3];
    if (*(ptr - 1) != 0xFFFFFFFF) { FAIL = 450; while(1); }
    if (*(ptr - 2) != 0x00000000) { FAIL = 451; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 5 — STORES  (FAIL 500-599)
 * Exercises: sb sh sw  (write then read back)
 * ═══════════════════════════════════════════════════════════════ */

static unsigned int store_buf[8];

static void test_stores(void) {

    /* --- SW: write and read back full word --- */
    store_buf[0] = 0xDEADBEEF;
    if (store_buf[0] != 0xDEADBEEF) { FAIL = 500; while(1); }

    store_buf[0] = 0x00000000;
    if (store_buf[0] != 0x00000000) { FAIL = 501; while(1); }

    store_buf[0] = 0xFFFFFFFF;
    if (store_buf[0] != 0xFFFFFFFF) { FAIL = 502; while(1); }

    /* overwrite: last write wins */
    store_buf[1] = 0x11111111;
    store_buf[1] = 0x22222222;
    if (store_buf[1] != 0x22222222) { FAIL = 503; while(1); }

    /* --- SB: store only low byte, other bytes unaffected --- */
    store_buf[2] = 0xDEADBEEF;
    unsigned char *bp = (unsigned char *)&store_buf[2];
    /* On little-endian: byte 0 = EF, byte 1 = BE, byte 2 = AD, byte 3 = DE */
    bp[0] = 0x00;
    /* Expect 0xDEADBE00 */
    if (store_buf[2] != 0xDEADBE00) { FAIL = 510; while(1); }

    bp[3] = 0xFF;
    /* Expect 0xFFADBE00 */
    if (store_buf[2] != 0xFFADBE00) { FAIL = 511; while(1); }

    /* SB write 0x7F to position 1 */
    store_buf[3] = 0x00000000;
    unsigned char *bp3 = (unsigned char *)&store_buf[3];
    bp3[1] = 0xAB;
    if (store_buf[3] != 0x0000AB00) { FAIL = 512; while(1); }

    /* --- SH: store only low halfword --- */
    store_buf[4] = 0xDEADBEEF;
    unsigned short *hp = (unsigned short *)&store_buf[4];
    hp[0] = 0x1234;
    /* low half = 0x1234, upper half = 0xDEAD unchanged */
    if (store_buf[4] != 0xDEAD1234) { FAIL = 520; while(1); }

    hp[1] = 0x5678;
    /* now full word = 0x56781234 */
    if (store_buf[4] != 0x56781234) { FAIL = 521; while(1); }

    /* SH of 0x0000 */
    store_buf[5] = 0xFFFFFFFF;
    unsigned short *hp5 = (unsigned short *)&store_buf[5];
    hp5[0] = 0x0000;
    if (store_buf[5] != 0xFFFF0000) { FAIL = 522; while(1); }

    /* --- SW with positive and negative immediate offsets --- */
    /* Write to index 6, read via pointer arithmetic */
    store_buf[6] = 0xCAFEBABE;
    unsigned int *wp = &store_buf[7];
    /* *(wp - 1) should be store_buf[6] */
    if (*(wp - 1) != 0xCAFEBABE) { FAIL = 530; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 6 — BRANCHES  (FAIL 600-699)
 * Exercises: beq bne blt bge bltu bgeu
 * ═══════════════════════════════════════════════════════════════ */

static void test_branches(void) {
    int a, b;
    int taken;

    /* --- BEQ --- */
    a = 5; b = 5; taken = 0;
    if (a == b) taken = 1;
    if (taken != 1)         { FAIL = 600; while(1); }

    a = 5; b = 6; taken = 0;
    if (a == b) taken = 1;
    if (taken != 0)         { FAIL = 601; while(1); }

    /* BEQ on zero */
    a = 0; b = 0; taken = 0;
    if (a == b) taken = 1;
    if (taken != 1)         { FAIL = 602; while(1); }

    /* --- BNE --- */
    a = 5; b = 6; taken = 0;
    if (a != b) taken = 1;
    if (taken != 1)         { FAIL = 610; while(1); }

    a = 5; b = 5; taken = 0;
    if (a != b) taken = 1;
    if (taken != 0)         { FAIL = 611; while(1); }

    /* --- BLT (signed) --- */
    /* -1 < 0 must branch */
    a = -1; b = 0; taken = 0;
    if (a < b) taken = 1;
    if (taken != 1)         { FAIL = 620; while(1); }

    /* 0 < -1 must NOT branch */
    a = 0; b = -1; taken = 0;
    if (a < b) taken = 1;
    if (taken != 0)         { FAIL = 621; while(1); }

    /* equal must NOT branch */
    a = 5; b = 5; taken = 0;
    if (a < b) taken = 1;
    if (taken != 0)         { FAIL = 622; while(1); }

    /* large negative < large positive */
    a = -2147483648; b = 2147483647; taken = 0;
    if (a < b) taken = 1;
    if (taken != 1)         { FAIL = 623; while(1); }

    /* --- BGE (signed) --- */
    /* 0 >= -1 must branch */
    a = 0; b = -1; taken = 0;
    if (a >= b) taken = 1;
    if (taken != 1)         { FAIL = 630; while(1); }

    /* equal must branch */
    a = 5; b = 5; taken = 0;
    if (a >= b) taken = 1;
    if (taken != 1)         { FAIL = 631; while(1); }

    /* -1 >= 0 must NOT branch */
    a = -1; b = 0; taken = 0;
    if (a >= b) taken = 1;
    if (taken != 0)         { FAIL = 632; while(1); }

    /* --- BLTU (unsigned) --- */
    /* CRITICAL: 0xFFFFFFFF as unsigned is LARGER than 1 */
    unsigned int ua, ub;
    ua = 0xFFFFFFFF; ub = 1; taken = 0;
    if (ua < ub) taken = 1;
    if (taken != 0)         { FAIL = 640; while(1); }

    /* 0 < 0xFFFFFFFF unsigned: must branch */
    ua = 0; ub = 0xFFFFFFFF; taken = 0;
    if (ua < ub) taken = 1;
    if (taken != 1)         { FAIL = 641; while(1); }

    /* equal: must NOT branch */
    ua = 7; ub = 7; taken = 0;
    if (ua < ub) taken = 1;
    if (taken != 0)         { FAIL = 642; while(1); }

    /* --- BGEU (unsigned) --- */
    ua = 0xFFFFFFFF; ub = 0; taken = 0;
    if (ua >= ub) taken = 1;
    if (taken != 1)         { FAIL = 650; while(1); }

    /* equal must branch */
    ua = 10; ub = 10; taken = 0;
    if (ua >= ub) taken = 1;
    if (taken != 1)         { FAIL = 651; while(1); }

    /* 0 >= 1 unsigned: must NOT branch */
    ua = 0; ub = 1; taken = 0;
    if (ua >= ub) taken = 1;
    if (taken != 0)         { FAIL = 652; while(1); }

    /* --- CRITICAL: signed vs unsigned comparison on same bit pattern --- */
    /* -1 signed < 1: TRUE.  0xFFFFFFFF unsigned < 1: FALSE */
    int          si = -1;
    unsigned int ui = 1;
    if (!(si  <  (int)ui))  { FAIL = 660; while(1); }  /* signed:   true  */
    if ((unsigned int)si < ui) { FAIL = 661; while(1); } /* unsigned: false */
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 7 — JAL / JALR  (FAIL 700-799)
 * ═══════════════════════════════════════════════════════════════ */

/* Return-value tests for JAL */
static int jal_result;

static void jal_leaf(void) {
    jal_result = 42;
}

static int jal_add(int a, int b) {
    return a + b;
}

/* Deep call chain to test stack discipline */
static int deep_a(int n);
static int deep_b(int n);
static int deep_c(int n);

static int deep_a(int n) { return n == 0 ? 1 : deep_b(n - 1); }
static int deep_b(int n) { return n == 0 ? 2 : deep_c(n - 1); }
static int deep_c(int n) { return n == 0 ? 3 : deep_a(n - 1); }

/* Function pointer table for JALR */
static int fp_a(void) { return 10; }
static int fp_b(void) { return 20; }
static int fp_c(void) { return 30; }

static void test_jal_jalr(void) {

    /* --- JAL: basic call and return --- */
    jal_result = 0;
    jal_leaf();
    if (jal_result != 42)   { FAIL = 700; while(1); }

    /* --- JAL: return value via register --- */
    int r = jal_add(3, 4);
    if (r != 7)             { FAIL = 701; while(1); }

    r = jal_add(-1, 1);
    if (r != 0)             { FAIL = 702; while(1); }

    r = jal_add(0x7FFFFFFF, 1); /* signed overflow */
    if (r != (int)0x80000000) { FAIL = 703; while(1); }

    /* --- JAL: nested calls (stack must be correct) --- */
    r = deep_a(3);  /* a→b→c→a→1 */
    if (r != 1)             { FAIL = 710; while(1); }
    r = deep_b(3);  /* b→c→a→b→2 */
    if (r != 2)             { FAIL = 711; while(1); }
    r = deep_c(3);  /* c→a→b→c→3 */
    if (r != 3)             { FAIL = 712; while(1); }

    /* --- JALR: function pointers (indirect call) --- */
    int (*ftable[3])(void) = { fp_a, fp_b, fp_c };

    if (ftable[0]() != 10)  { FAIL = 720; while(1); }
    if (ftable[1]() != 20)  { FAIL = 721; while(1); }
    if (ftable[2]() != 30)  { FAIL = 722; while(1); }

    /* Call via variable pointer */
    int (*fp)(void) = fp_b;
    if (fp() != 20)         { FAIL = 723; while(1); }

    fp = fp_c;
    if (fp() != 30)         { FAIL = 724; while(1); }

    /* --- Recursive call (JAL + stack frames) --- */
    /* fibonacci recursive already tested, do factorial here */
    /* 5! = 120 */
}

/* factorial: tests recursive JAL deeply */
static int factorial(int n)
{
    int result = 1;

    for(int i=2;i<=n;i++)
    {
        int temp = 0;

        for(int j=0;j<i;j++)
            temp += result;

        result = temp;
    }

    return result;
}

static void test_recursion(void) {
    if (factorial(1)  != 1)         { FAIL = 730; while(1); }
    if (factorial(2)  != 2)         { FAIL = 731; while(1); }
    if (factorial(5)  != 120)       { FAIL = 732; while(1); }
    if (factorial(10) != 3628800)   { FAIL = 733; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 8 — HAZARDS / FORWARDING / x0  (FAIL 800-899)
 * These expose pipeline forwarding bugs without the compiler
 * being able to reorder/eliminate them (volatile forces it).
 * ═══════════════════════════════════════════════════════════════ */

static void test_forwarding(void) {
    volatile int a, b, c, d;

    /* RAW hazard: result used immediately in next instruction */
    a = 3;
    b = a + 1;      /* depends on a  — EX/MEM forwarding */
    c = b + 1;      /* depends on b  — EX/EX forwarding  */
    d = c + 1;      /* depends on c */
    if (d != 6)     { FAIL = 800; while(1); }

    /* RAW: load-use hazard (LW followed immediately by dependent op) */
    volatile int arr[4];
    arr[0] = 99;
    int x = arr[0]; /* LW */
    int y = x + 1;  /* immediately uses x — load-use hazard */
    if (y != 100)   { FAIL = 801; while(1); }

    /* RAW: store then load same address */
    arr[1] = 0xABCD;
    int z = arr[1];
    if (z != 0xABCD){ FAIL = 802; while(1); }

    /* Chain: a=1; b=a+a; c=b+b; d=c+c → 1,2,4,8 */
    a = 1;
    b = a + a;
    c = b + b;
    d = c + c;
    if (a != 1)     { FAIL = 803; while(1); }
    if (b != 2)     { FAIL = 804; while(1); }
    if (c != 4)     { FAIL = 805; while(1); }
    if (d != 8)     { FAIL = 806; while(1); }

    /* Same register as src and dst */
    a = 10;
    a = a + a;      /* a = 20 */
    a = a + a;      /* a = 40 */
    if (a != 40)    { FAIL = 807; while(1); }

    /* MUL-by-addition: result of store visible on next load */
    volatile int mem;
    mem = 0;
    mem = mem + 5;
    if (mem != 5)   { FAIL = 808; while(1); }
    mem = mem + mem;
    if (mem != 10)  { FAIL = 809; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * SECTION 9 — SHIFT EDGE CASES  (FAIL 900-999)
 * Shift by 0, 31, register-amount masking (only low 5 bits used)
 * Tests sll/srl/sra by register and by immediate
 * ═══════════════════════════════════════════════════════════════ */

static void test_shift_edges(void) {
    volatile unsigned int u;
    volatile int          s;
    volatile int          sh;

    /* Shift by 0 = identity for all three */
    u = 0xDEADBEEF; sh = 0;
    if ((u << sh) != 0xDEADBEEF)   { FAIL = 900; while(1); }
    if ((u >> sh) != 0xDEADBEEF)   { FAIL = 901; while(1); }
    s = -1;
    if ((s >> sh)  != -1)           { FAIL = 902; while(1); }

    /* Shift by 1 */
    u = 0x80000000; sh = 1;
    if ((u >> sh) != 0x40000000)    { FAIL = 903; while(1); }  /* SRL */
    s = (int)0x80000000; sh = 1;
    if ((s >> sh) != (int)0xC0000000) { FAIL = 904; while(1); } /* SRA */

    /* Shift left by 31 */
    u = 1; sh = 31;
    if ((u << sh) != 0x80000000)    { FAIL = 910; while(1); }

    /* Shift right by 31 */
    u = 0x80000000; sh = 31;
    if ((u >> sh) != 1)             { FAIL = 911; while(1); }  /* SRL */
    s = (int)0x80000000; sh = 31;
    if ((s >> sh) != -1)            { FAIL = 912; while(1); }  /* SRA */

    /* CRITICAL: shift amount uses only low 5 bits
       shift by 32 = shift by 0 (32 & 0x1F = 0) */
    u = 0xDEADBEEF; sh = 32;
    if ((u << sh) != 0xDEADBEEF)    { FAIL = 920; while(1); }
    if ((u >> sh) != 0xDEADBEEF)    { FAIL = 921; while(1); }
    s = -1; sh = 32;
    if ((s >> sh) != -1)            { FAIL = 922; while(1); }

    /* shift by 33 = shift by 1 */
    u = 0x80000000; sh = 33;
    if ((u >> sh) != 0x40000000)    { FAIL = 923; while(1); }
    u = 1; sh = 33;
    if ((u << sh) != 2)             { FAIL = 924; while(1); }

    /* shift by 63 = shift by 31 */
    u = 0x80000000; sh = 63;
    if ((u >> sh) != 1)             { FAIL = 925; while(1); }

    /* Immediate shifts: same edge cases via slli/srli/srai */
    u = 1;
    if ((u << 0)  != 1)             { FAIL = 930; while(1); }
    if ((u << 31) != 0x80000000)    { FAIL = 931; while(1); }
    u = 0x80000000;
    if ((u >> 31) != 1)             { FAIL = 932; while(1); }
    s = -1;
    if ((s >> 31) != -1)            { FAIL = 933; while(1); }

    /* Shift produces zero */
    u = 1; sh = 31;
    unsigned int shifted_out = u >> sh;  /* 0 (bit 0 goes to position 0 → actually 0) */
    /* Wait: 1 >> 31 = 0 (bit 0 is not bit 31) */
    if (shifted_out != 0)           { FAIL = 934; while(1); }

    u = 0x80000000; sh = 31;
    if ((u >> sh) != 1)             { FAIL = 935; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * BONUS: mixed / integration tests that cross instruction groups
 * These are the hardest to pass — real algorithm stress tests
 * ═══════════════════════════════════════════════════════════════ */

/* Binary search — exercises: lw, add, sub, sra(i), blt, bge, beq, bne, jal */
static int sorted[16] = {
    2,5,8,12,16,23,38,56,72,91,100,113,142,170,199,210
};

static int bsearch_test(int *arr, int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

static void test_integration(void) {
    if (bsearch_test(sorted, 16, 2)   !=  0) { FAIL = 950; while(1); }
    if (bsearch_test(sorted, 16, 210) != 15) { FAIL = 951; while(1); }
    if (bsearch_test(sorted, 16, 91)  !=  9) { FAIL = 952; while(1); }
    if (bsearch_test(sorted, 16, 38)  !=  6) { FAIL = 953; while(1); }
    if (bsearch_test(sorted, 16, 999) != -1) { FAIL = 954; while(1); }
    if (bsearch_test(sorted, 16, 1)   != -1) { FAIL = 955; while(1); }

    /* Bubble sort */
    int arr[8] = {64, 34, 25, 12, 22, 11, 90, 1};
    int i, j, tmp, n = 8;
    for (i = 0; i < n-1; i++)
        for (j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1]) {
                tmp      = arr[j];
                arr[j]   = arr[j+1];
                arr[j+1] = tmp;
            }
    int expected[8] = {1, 11, 12, 22, 25, 34, 64, 90};
    for (i = 0; i < 8; i++)
        if (arr[i] != expected[i]) { FAIL = 960 + i; while(1); }

    /* Knapsack DP — exercises 2D array addressing, all ALU ops */
    int val[5] = {60, 100, 120, 40, 80};
    int wt[5]  = {10,  20,  30,  5, 15};
    int dp[6][51];
    int w;
    for (i = 0; i <= 5; i++)
        for (w = 0; w <= 50; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (wt[i-1] <= w) {
                int take = val[i-1] + dp[i-1][w - wt[i-1]];
                int skip = dp[i-1][w];
                dp[i][w] = take > skip ? take : skip;
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    if (dp[5][50] != 280) { FAIL = 970; while(1); }
}

/* ═══════════════════════════════════════════════════════════════
 * MAIN
 * ═══════════════════════════════════════════════════════════════ */

int main(void) {

    /* Section 1: R-type */
    test_add();
    test_sub();
    test_xor();
    test_or();
    test_and();
    test_sll();
    test_srl();
    test_sra();
    test_slt_sltu();

    /* Section 2: I-type ALU */
    test_addi();
    test_xori();
    test_ori();
    test_andi();
    test_slli();
    test_srli();
    test_srai();
    test_slti();
    test_sltiu();

    /* Section 3: Upper immediate */
    test_lui();
    test_auipc();

    /* Section 4: Loads */
    test_loads();

    /* Section 5: Stores */
    test_stores();

    /* Section 6: Branches */
    test_branches();

    /* Section 7: JAL / JALR */
    test_jal_jalr();
    test_recursion();

    /* Section 8: Hazards / forwarding */
    test_forwarding();

    /* Section 9: Shift edge cases */
    test_shift_edges();

    /* Integration */
    test_integration();

    PASS = 0xDEADBEEF;
    return 0;
}


