/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(JIT)
#include "JIT.h"

#include "CodeBlock.h"
#include "JITInlines.h"
#include "JITOperations.h"
#include "JITStubs.h"
#include "JSArray.h"
#include "JSFunction.h"
#include "Interpreter.h"
#include "JSCInlines.h"
#include "ResultType.h"
#include "SamplingTool.h"
#include "SlowPathCall.h"


namespace JSC {

void JIT::emit_op_jless(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jless, op1, op2, target, LessThan);
}

void JIT::emit_op_jlesseq(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jlesseq, op1, op2, target, LessThanOrEqual);
}

void JIT::emit_op_jgreater(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jgreater, op1, op2, target, GreaterThan);
}

void JIT::emit_op_jgreatereq(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jgreatereq, op1, op2, target, GreaterThanOrEqual);
}

void JIT::emit_op_jnless(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jnless, op1, op2, target, GreaterThanOrEqual);
}

void JIT::emit_op_jnlesseq(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jnlesseq, op1, op2, target, GreaterThan);
}

void JIT::emit_op_jngreater(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jngreater, op1, op2, target, LessThanOrEqual);
}

void JIT::emit_op_jngreatereq(Instruction* currentInstruction)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJump(op_jngreatereq, op1, op2, target, LessThan);
}

void JIT::emitSlow_op_jless(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleLessThan, operationCompareLess, false, iter);
}

void JIT::emitSlow_op_jlesseq(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleLessThanOrEqual, operationCompareLessEq, false, iter);
}

void JIT::emitSlow_op_jgreater(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleGreaterThan, operationCompareGreater, false, iter);
}

void JIT::emitSlow_op_jgreatereq(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleGreaterThanOrEqual, operationCompareGreaterEq, false, iter);
}

void JIT::emitSlow_op_jnless(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleGreaterThanOrEqualOrUnordered, operationCompareLess, true, iter);
}

void JIT::emitSlow_op_jnlesseq(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleGreaterThanOrUnordered, operationCompareLessEq, true, iter);
}

void JIT::emitSlow_op_jngreater(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleLessThanOrEqualOrUnordered, operationCompareGreater, true, iter);
}

void JIT::emitSlow_op_jngreatereq(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[1].u.operand;
    int op2 = currentInstruction[2].u.operand;
    unsigned target = currentInstruction[3].u.operand;

    emit_compareAndJumpSlow(op1, op2, target, DoubleLessThanOrUnordered, operationCompareGreaterEq, true, iter);
}

#if USE(JSVALUE64)

void JIT::emit_op_negate(Instruction* currentInstruction)
{
    int dst = currentInstruction[1].u.operand;
    int src = currentInstruction[2].u.operand;

    emitGetVirtualRegister(src, regT0);

    Jump srcNotInt = emitJumpIfNotImmediateInteger(regT0);
    addSlowCase(branchTest32(Zero, regT0, TrustedImm32(0x7fffffff)));
    neg32(regT0);
    emitFastArithReTagImmediate(regT0, regT0);

    Jump end = jump();

    srcNotInt.link(this);
    emitJumpSlowCaseIfNotImmediateNumber(regT0);

    move(TrustedImm64((int64_t)0x8000000000000000ull), regT1);
    xor64(regT1, regT0);

    end.link(this);
    emitPutVirtualRegister(dst);
}

void JIT::emitSlow_op_negate(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    linkSlowCase(iter); // 0x7fffffff check
    linkSlowCase(iter); // double check

    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_negate);
    slowPathCall.call();
}

void JIT::emit_op_lshift(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;

    emitGetVirtualRegisters(op1, regT0, op2, regT2);
    // FIXME: would we be better using 'emitJumpSlowCaseIfNotImmediateIntegers'? - we *probably* ought to be consistent.
    emitJumpSlowCaseIfNotImmediateInteger(regT0);
    emitJumpSlowCaseIfNotImmediateInteger(regT2);
    emitFastArithImmToInt(regT0);
    emitFastArithImmToInt(regT2);
    lshift32(regT2, regT0);
    emitFastArithReTagImmediate(regT0, regT0);
    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_lshift(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    linkSlowCase(iter);
    linkSlowCase(iter);
    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_lshift);
    slowPathCall.call();
}

void JIT::emit_op_rshift(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;

    if (isOperandConstantImmediateInt(op2)) {
        // isOperandConstantImmediateInt(op2) => 1 SlowCase
        emitGetVirtualRegister(op1, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        // Mask with 0x1f as per ecma-262 11.7.2 step 7.
        rshift32(Imm32(getConstantOperandImmediateInt(op2) & 0x1f), regT0);
    } else {
        emitGetVirtualRegisters(op1, regT0, op2, regT2);
        if (supportsFloatingPointTruncate()) {
            Jump lhsIsInt = emitJumpIfImmediateInteger(regT0);
            // supportsFloatingPoint() && USE(JSVALUE64) => 3 SlowCases
            addSlowCase(emitJumpIfNotImmediateNumber(regT0));
            add64(tagTypeNumberRegister, regT0);
            move64ToDouble(regT0, fpRegT0);
            addSlowCase(branchTruncateDoubleToInt32(fpRegT0, regT0));
            lhsIsInt.link(this);
            emitJumpSlowCaseIfNotImmediateInteger(regT2);
        } else {
            // !supportsFloatingPoint() => 2 SlowCases
            emitJumpSlowCaseIfNotImmediateInteger(regT0);
            emitJumpSlowCaseIfNotImmediateInteger(regT2);
        }
        emitFastArithImmToInt(regT2);
        rshift32(regT2, regT0);
    }
    emitFastArithIntToImmNoCheck(regT0, regT0);
    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_rshift(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op2 = currentInstruction[3].u.operand;

    if (isOperandConstantImmediateInt(op2))
        linkSlowCase(iter);

    else {
        if (supportsFloatingPointTruncate()) {
            linkSlowCase(iter);
            linkSlowCase(iter);
            linkSlowCase(iter);
        } else {
            linkSlowCase(iter);
            linkSlowCase(iter);
        }
    }

    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_rshift);
    slowPathCall.call();
}

void JIT::emit_op_urshift(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;

    if (isOperandConstantImmediateInt(op2)) {
        // isOperandConstantImmediateInt(op2) => 1 SlowCase
        emitGetVirtualRegister(op1, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        // Mask with 0x1f as per ecma-262 11.7.2 step 7.
        urshift32(Imm32(getConstantOperandImmediateInt(op2) & 0x1f), regT0);
    } else {
        emitGetVirtualRegisters(op1, regT0, op2, regT2);
        if (supportsFloatingPointTruncate()) {
            Jump lhsIsInt = emitJumpIfImmediateInteger(regT0);
            // supportsFloatingPoint() && USE(JSVALUE64) => 3 SlowCases
            addSlowCase(emitJumpIfNotImmediateNumber(regT0));
            add64(tagTypeNumberRegister, regT0);
            move64ToDouble(regT0, fpRegT0);
            addSlowCase(branchTruncateDoubleToInt32(fpRegT0, regT0));
            lhsIsInt.link(this);
            emitJumpSlowCaseIfNotImmediateInteger(regT2);
        } else {
            // !supportsFloatingPoint() => 2 SlowCases
            emitJumpSlowCaseIfNotImmediateInteger(regT0);
            emitJumpSlowCaseIfNotImmediateInteger(regT2);
        }
        emitFastArithImmToInt(regT2);
        urshift32(regT2, regT0);
    }
    emitFastArithIntToImmNoCheck(regT0, regT0);
    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_urshift(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op2 = currentInstruction[3].u.operand;

    if (isOperandConstantImmediateInt(op2))
        linkSlowCase(iter);

    else {
        if (supportsFloatingPointTruncate()) {
            linkSlowCase(iter);
            linkSlowCase(iter);
            linkSlowCase(iter);
        } else {
            linkSlowCase(iter);
            linkSlowCase(iter);
        }
    }

    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_urshift);
    slowPathCall.call();
}

void JIT::emit_op_unsigned(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    
    emitGetVirtualRegister(op1, regT0);
    emitJumpSlowCaseIfNotImmediateInteger(regT0);
    addSlowCase(branch32(LessThan, regT0, TrustedImm32(0)));
    emitFastArithReTagImmediate(regT0, regT0);
    emitPutVirtualRegister(result, regT0);
}

void JIT::emitSlow_op_unsigned(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    linkSlowCase(iter);
    linkSlowCase(iter);
    
    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_unsigned);
    slowPathCall.call();
}

void JIT::emit_compareAndJump(OpcodeID, int op1, int op2, unsigned target, RelationalCondition condition)
{
    // We generate inline code for the following cases in the fast path:
    // - int immediate to constant int immediate
    // - constant int immediate to int immediate
    // - int immediate to int immediate

    if (isOperandConstantImmediateChar(op1)) {
        emitGetVirtualRegister(op2, regT0);
        addSlowCase(emitJumpIfNotJSCell(regT0));
        JumpList failures;
        emitLoadCharacterString(regT0, regT0, failures);
        addSlowCase(failures);
        addJump(branch32(commute(condition), regT0, Imm32(asString(getConstantOperand(op1))->tryGetValue()[0])), target);
        return;
    }
    if (isOperandConstantImmediateChar(op2)) {
        emitGetVirtualRegister(op1, regT0);
        addSlowCase(emitJumpIfNotJSCell(regT0));
        JumpList failures;
        emitLoadCharacterString(regT0, regT0, failures);
        addSlowCase(failures);
        addJump(branch32(condition, regT0, Imm32(asString(getConstantOperand(op2))->tryGetValue()[0])), target);
        return;
    }
    if (isOperandConstantImmediateInt(op2)) {
        emitGetVirtualRegister(op1, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        int32_t op2imm = getConstantOperandImmediateInt(op2);
        addJump(branch32(condition, regT0, Imm32(op2imm)), target);
    } else if (isOperandConstantImmediateInt(op1)) {
        emitGetVirtualRegister(op2, regT1);
        emitJumpSlowCaseIfNotImmediateInteger(regT1);
        int32_t op1imm = getConstantOperandImmediateInt(op1);
        addJump(branch32(commute(condition), regT1, Imm32(op1imm)), target);
    } else {
        emitGetVirtualRegisters(op1, regT0, op2, regT1);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT1);

        addJump(branch32(condition, regT0, regT1), target);
    }
}

void JIT::emit_compareAndJumpSlow(int op1, int op2, unsigned target, DoubleCondition condition, size_t (JIT_OPERATION *operation)(ExecState*, EncodedJSValue, EncodedJSValue), bool invert, Vector<SlowCaseEntry>::iterator& iter)
{
    COMPILE_ASSERT(OPCODE_LENGTH(op_jless) == OPCODE_LENGTH(op_jlesseq), OPCODE_LENGTH_op_jlesseq_equals_op_jless);
    COMPILE_ASSERT(OPCODE_LENGTH(op_jless) == OPCODE_LENGTH(op_jnless), OPCODE_LENGTH_op_jnless_equals_op_jless);
    COMPILE_ASSERT(OPCODE_LENGTH(op_jless) == OPCODE_LENGTH(op_jnlesseq), OPCODE_LENGTH_op_jnlesseq_equals_op_jless);
    COMPILE_ASSERT(OPCODE_LENGTH(op_jless) == OPCODE_LENGTH(op_jgreater), OPCODE_LENGTH_op_jgreater_equals_op_jless);
    COMPILE_ASSERT(OPCODE_LENGTH(op_jless) == OPCODE_LENGTH(op_jgreatereq), OPCODE_LENGTH_op_jgreatereq_equals_op_jless);
    COMPILE_ASSERT(OPCODE_LENGTH(op_jless) == OPCODE_LENGTH(op_jngreater), OPCODE_LENGTH_op_jngreater_equals_op_jless);
    COMPILE_ASSERT(OPCODE_LENGTH(op_jless) == OPCODE_LENGTH(op_jngreatereq), OPCODE_LENGTH_op_jngreatereq_equals_op_jless);
    
    // We generate inline code for the following cases in the slow path:
    // - floating-point number to constant int immediate
    // - constant int immediate to floating-point number
    // - floating-point number to floating-point number.
    if (isOperandConstantImmediateChar(op1) || isOperandConstantImmediateChar(op2)) {
        linkSlowCase(iter);
        linkSlowCase(iter);
        linkSlowCase(iter);
        linkSlowCase(iter);

        emitGetVirtualRegister(op1, argumentGPR0);
        emitGetVirtualRegister(op2, argumentGPR1);
        callOperation(operation, argumentGPR0, argumentGPR1);
        emitJumpSlowToHot(branchTest32(invert ? Zero : NonZero, returnValueGPR), target);
        return;
    }

    if (isOperandConstantImmediateInt(op2)) {
        linkSlowCase(iter);

        if (supportsFloatingPoint()) {
            Jump fail1 = emitJumpIfNotImmediateNumber(regT0);
            add64(tagTypeNumberRegister, regT0);
            move64ToDouble(regT0, fpRegT0);

            int32_t op2imm = getConstantOperand(op2).asInt32();

            move(Imm32(op2imm), regT1);
            convertInt32ToDouble(regT1, fpRegT1);

            emitJumpSlowToHot(branchDouble(condition, fpRegT0, fpRegT1), target);

            emitJumpSlowToHot(jump(), OPCODE_LENGTH(op_jless));

            fail1.link(this);
        }

        emitGetVirtualRegister(op2, regT1);
        callOperation(operation, regT0, regT1);
        emitJumpSlowToHot(branchTest32(invert ? Zero : NonZero, returnValueGPR), target);
    } else if (isOperandConstantImmediateInt(op1)) {
        linkSlowCase(iter);

        if (supportsFloatingPoint()) {
            Jump fail1 = emitJumpIfNotImmediateNumber(regT1);
            add64(tagTypeNumberRegister, regT1);
            move64ToDouble(regT1, fpRegT1);

            int32_t op1imm = getConstantOperand(op1).asInt32();

            move(Imm32(op1imm), regT0);
            convertInt32ToDouble(regT0, fpRegT0);

            emitJumpSlowToHot(branchDouble(condition, fpRegT0, fpRegT1), target);

            emitJumpSlowToHot(jump(), OPCODE_LENGTH(op_jless));

            fail1.link(this);
        }

        emitGetVirtualRegister(op1, regT2);
        callOperation(operation, regT2, regT1);
        emitJumpSlowToHot(branchTest32(invert ? Zero : NonZero, returnValueGPR), target);
    } else {
        linkSlowCase(iter);

        if (supportsFloatingPoint()) {
            Jump fail1 = emitJumpIfNotImmediateNumber(regT0);
            Jump fail2 = emitJumpIfNotImmediateNumber(regT1);
            Jump fail3 = emitJumpIfImmediateInteger(regT1);
            add64(tagTypeNumberRegister, regT0);
            add64(tagTypeNumberRegister, regT1);
            move64ToDouble(regT0, fpRegT0);
            move64ToDouble(regT1, fpRegT1);

            emitJumpSlowToHot(branchDouble(condition, fpRegT0, fpRegT1), target);

            emitJumpSlowToHot(jump(), OPCODE_LENGTH(op_jless));

            fail1.link(this);
            fail2.link(this);
            fail3.link(this);
        }

        linkSlowCase(iter);
        callOperation(operation, regT0, regT1);
        emitJumpSlowToHot(branchTest32(invert ? Zero : NonZero, returnValueGPR), target);
    }
}

void JIT::emit_op_bitand(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;

    if (isOperandConstantImmediateInt(op1)) {
        emitGetVirtualRegister(op2, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        int32_t imm = getConstantOperandImmediateInt(op1);
        and64(Imm32(imm), regT0);
        if (imm >= 0)
            emitFastArithIntToImmNoCheck(regT0, regT0);
    } else if (isOperandConstantImmediateInt(op2)) {
        emitGetVirtualRegister(op1, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        int32_t imm = getConstantOperandImmediateInt(op2);
        and64(Imm32(imm), regT0);
        if (imm >= 0)
            emitFastArithIntToImmNoCheck(regT0, regT0);
    } else {
        emitGetVirtualRegisters(op1, regT0, op2, regT1);
        and64(regT1, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
    }
    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_bitand(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    linkSlowCase(iter);

    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_bitand);
    slowPathCall.call();
}

void JIT::emit_op_inc(Instruction* currentInstruction)
{
    int srcDst = currentInstruction[1].u.operand;

    emitGetVirtualRegister(srcDst, regT0);
    emitJumpSlowCaseIfNotImmediateInteger(regT0);
    addSlowCase(branchAdd32(Overflow, TrustedImm32(1), regT0));
    emitFastArithIntToImmNoCheck(regT0, regT0);
    emitPutVirtualRegister(srcDst);
}

void JIT::emitSlow_op_inc(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    linkSlowCase(iter);
    linkSlowCase(iter);
    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_inc);
    slowPathCall.call();
}

void JIT::emit_op_dec(Instruction* currentInstruction)
{
    int srcDst = currentInstruction[1].u.operand;

    emitGetVirtualRegister(srcDst, regT0);
    emitJumpSlowCaseIfNotImmediateInteger(regT0);
    addSlowCase(branchSub32(Overflow, TrustedImm32(1), regT0));
    emitFastArithIntToImmNoCheck(regT0, regT0);
    emitPutVirtualRegister(srcDst);
}

void JIT::emitSlow_op_dec(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    linkSlowCase(iter);
    linkSlowCase(iter);
    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_dec);
    slowPathCall.call();
}

/* ------------------------------ BEGIN: OP_MOD ------------------------------ */

#if CPU(X86) || CPU(X86_64)

void JIT::emit_op_mod(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;

    // Make sure registers are correct for x86 IDIV instructions.
    ASSERT(regT0 == X86Registers::eax);
    ASSERT(regT1 == X86Registers::edx);
    ASSERT(regT2 == X86Registers::ecx);

    emitGetVirtualRegisters(op1, regT3, op2, regT2);
    emitJumpSlowCaseIfNotImmediateInteger(regT3);
    emitJumpSlowCaseIfNotImmediateInteger(regT2);

    move(regT3, regT0);
    addSlowCase(branchTest32(Zero, regT2));
    Jump denominatorNotNeg1 = branch32(NotEqual, regT2, TrustedImm32(-1));
    addSlowCase(branch32(Equal, regT0, TrustedImm32(-2147483647-1)));
    denominatorNotNeg1.link(this);
    m_assembler.cdq();
    m_assembler.idivl_r(regT2);
    Jump numeratorPositive = branch32(GreaterThanOrEqual, regT3, TrustedImm32(0));
    addSlowCase(branchTest32(Zero, regT1));
    numeratorPositive.link(this);
    emitFastArithReTagImmediate(regT1, regT0);
    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_mod(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    linkSlowCase(iter);
    linkSlowCase(iter);
    linkSlowCase(iter);
    linkSlowCase(iter);
    linkSlowCase(iter);
    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_mod);
    slowPathCall.call();
}

#else // CPU(X86) || CPU(X86_64)

void JIT::emit_op_mod(Instruction* currentInstruction)
{
    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_mod);
    slowPathCall.call();
}

void JIT::emitSlow_op_mod(Instruction*, Vector<SlowCaseEntry>::iterator&)
{
    UNREACHABLE_FOR_PLATFORM();
}

#endif // CPU(X86) || CPU(X86_64)

/* ------------------------------ END: OP_MOD ------------------------------ */

/* ------------------------------ BEGIN: USE(JSVALUE64) (OP_ADD, OP_SUB, OP_MUL) ------------------------------ */

void JIT::compileBinaryArithOp(OpcodeID opcodeID, int, int op1, int op2, OperandTypes)
{
    emitGetVirtualRegisters(op1, regT0, op2, regT1);
    emitJumpSlowCaseIfNotImmediateInteger(regT0);
    emitJumpSlowCaseIfNotImmediateInteger(regT1);
    RareCaseProfile* profile = m_codeBlock->addSpecialFastCaseProfile(m_bytecodeOffset);
    if (opcodeID == op_add)
        addSlowCase(branchAdd32(Overflow, regT1, regT0));
    else if (opcodeID == op_sub)
        addSlowCase(branchSub32(Overflow, regT1, regT0));
    else {
        ASSERT(opcodeID == op_mul);
        if (shouldEmitProfiling()) {
            // We want to be able to measure if this is taking the slow case just
            // because of negative zero. If this produces positive zero, then we
            // don't want the slow case to be taken because that will throw off
            // speculative compilation.
            move(regT0, regT2);
            addSlowCase(branchMul32(Overflow, regT1, regT2));
            JumpList done;
            done.append(branchTest32(NonZero, regT2));
            Jump negativeZero = branch32(LessThan, regT0, TrustedImm32(0));
            done.append(branch32(GreaterThanOrEqual, regT1, TrustedImm32(0)));
            negativeZero.link(this);
            // We only get here if we have a genuine negative zero. Record this,
            // so that the speculative JIT knows that we failed speculation
            // because of a negative zero.
            add32(TrustedImm32(1), AbsoluteAddress(&profile->m_counter));
            addSlowCase(jump());
            done.link(this);
            move(regT2, regT0);
        } else {
            addSlowCase(branchMul32(Overflow, regT1, regT0));
            addSlowCase(branchTest32(Zero, regT0));
        }
    }
    emitFastArithIntToImmNoCheck(regT0, regT0);
}

void JIT::compileBinaryArithOpSlowCase(Instruction* currentInstruction, OpcodeID opcodeID, Vector<SlowCaseEntry>::iterator& iter, int result, int op1, int op2, OperandTypes types, bool op1HasImmediateIntFastCase, bool op2HasImmediateIntFastCase)
{
    // We assume that subtracting TagTypeNumber is equivalent to adding DoubleEncodeOffset.
    COMPILE_ASSERT(((TagTypeNumber + DoubleEncodeOffset) == 0), TagTypeNumber_PLUS_DoubleEncodeOffset_EQUALS_0);

    Jump notImm1;
    Jump notImm2;
    if (op1HasImmediateIntFastCase) {
        notImm2 = getSlowCase(iter);
    } else if (op2HasImmediateIntFastCase) {
        notImm1 = getSlowCase(iter);
    } else {
        notImm1 = getSlowCase(iter);
        notImm2 = getSlowCase(iter);
    }

    linkSlowCase(iter); // Integer overflow case - we could handle this in JIT code, but this is likely rare.
    if (opcodeID == op_mul && !op1HasImmediateIntFastCase && !op2HasImmediateIntFastCase) // op_mul has an extra slow case to handle 0 * negative number.
        linkSlowCase(iter);

    Label stubFunctionCall(this);

    JITSlowPathCall slowPathCall(this, currentInstruction, opcodeID == op_add ? slow_path_add : opcodeID == op_sub ? slow_path_sub : slow_path_mul);
    slowPathCall.call();
    Jump end = jump();

    if (op1HasImmediateIntFastCase) {
        notImm2.link(this);
        if (!types.second().definitelyIsNumber())
            emitJumpIfNotImmediateNumber(regT0).linkTo(stubFunctionCall, this);
        emitGetVirtualRegister(op1, regT1);
        convertInt32ToDouble(regT1, fpRegT1);
        add64(tagTypeNumberRegister, regT0);
        move64ToDouble(regT0, fpRegT2);
    } else if (op2HasImmediateIntFastCase) {
        notImm1.link(this);
        if (!types.first().definitelyIsNumber())
            emitJumpIfNotImmediateNumber(regT0).linkTo(stubFunctionCall, this);
        emitGetVirtualRegister(op2, regT1);
        convertInt32ToDouble(regT1, fpRegT1);
        add64(tagTypeNumberRegister, regT0);
        move64ToDouble(regT0, fpRegT2);
    } else {
        // if we get here, eax is not an int32, edx not yet checked.
        notImm1.link(this);
        if (!types.first().definitelyIsNumber())
            emitJumpIfNotImmediateNumber(regT0).linkTo(stubFunctionCall, this);
        if (!types.second().definitelyIsNumber())
            emitJumpIfNotImmediateNumber(regT1).linkTo(stubFunctionCall, this);
        add64(tagTypeNumberRegister, regT0);
        move64ToDouble(regT0, fpRegT1);
        Jump op2isDouble = emitJumpIfNotImmediateInteger(regT1);
        convertInt32ToDouble(regT1, fpRegT2);
        Jump op2wasInteger = jump();

        // if we get here, eax IS an int32, edx is not.
        notImm2.link(this);
        if (!types.second().definitelyIsNumber())
            emitJumpIfNotImmediateNumber(regT1).linkTo(stubFunctionCall, this);
        convertInt32ToDouble(regT0, fpRegT1);
        op2isDouble.link(this);
        add64(tagTypeNumberRegister, regT1);
        move64ToDouble(regT1, fpRegT2);
        op2wasInteger.link(this);
    }

    if (opcodeID == op_add)
        addDouble(fpRegT2, fpRegT1);
    else if (opcodeID == op_sub)
        subDouble(fpRegT2, fpRegT1);
    else if (opcodeID == op_mul)
        mulDouble(fpRegT2, fpRegT1);
    else {
        ASSERT(opcodeID == op_div);
        divDouble(fpRegT2, fpRegT1);
    }
    moveDoubleTo64(fpRegT1, regT0);
    sub64(tagTypeNumberRegister, regT0);
    emitPutVirtualRegister(result, regT0);

    end.link(this);
}

void JIT::emit_op_add(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);

    if (!types.first().mightBeNumber() || !types.second().mightBeNumber()) {
        addSlowCase();
        JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_add);
        slowPathCall.call();
        return;
    }

    if (isOperandConstantImmediateInt(op1)) {
        emitGetVirtualRegister(op2, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        addSlowCase(branchAdd32(Overflow, regT0, Imm32(getConstantOperandImmediateInt(op1)), regT1));
        emitFastArithIntToImmNoCheck(regT1, regT0);
    } else if (isOperandConstantImmediateInt(op2)) {
        emitGetVirtualRegister(op1, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        addSlowCase(branchAdd32(Overflow, regT0, Imm32(getConstantOperandImmediateInt(op2)), regT1));
        emitFastArithIntToImmNoCheck(regT1, regT0);
    } else
        compileBinaryArithOp(op_add, result, op1, op2, types);

    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_add(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);

    if (!types.first().mightBeNumber() || !types.second().mightBeNumber()) {
        linkDummySlowCase(iter);
        return;
    }

    bool op1HasImmediateIntFastCase = isOperandConstantImmediateInt(op1);
    bool op2HasImmediateIntFastCase = !op1HasImmediateIntFastCase && isOperandConstantImmediateInt(op2);
    compileBinaryArithOpSlowCase(currentInstruction, op_add, iter, result, op1, op2, types, op1HasImmediateIntFastCase, op2HasImmediateIntFastCase);
}

void JIT::emit_op_mul(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);

    // For now, only plant a fast int case if the constant operand is greater than zero.
    int32_t value;
    if (isOperandConstantImmediateInt(op1) && ((value = getConstantOperandImmediateInt(op1)) > 0)) {
        // Add a special fast case profile because the DFG JIT will expect one.
        m_codeBlock->addSpecialFastCaseProfile(m_bytecodeOffset);
        emitGetVirtualRegister(op2, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        addSlowCase(branchMul32(Overflow, Imm32(value), regT0, regT1));
        emitFastArithReTagImmediate(regT1, regT0);
    } else if (isOperandConstantImmediateInt(op2) && ((value = getConstantOperandImmediateInt(op2)) > 0)) {
        // Add a special fast case profile because the DFG JIT will expect one.
        m_codeBlock->addSpecialFastCaseProfile(m_bytecodeOffset);
        emitGetVirtualRegister(op1, regT0);
        emitJumpSlowCaseIfNotImmediateInteger(regT0);
        addSlowCase(branchMul32(Overflow, Imm32(value), regT0, regT1));
        emitFastArithReTagImmediate(regT1, regT0);
    } else
        compileBinaryArithOp(op_mul, result, op1, op2, types);

    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_mul(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);

    bool op1HasImmediateIntFastCase = isOperandConstantImmediateInt(op1) && getConstantOperandImmediateInt(op1) > 0;
    bool op2HasImmediateIntFastCase = !op1HasImmediateIntFastCase && isOperandConstantImmediateInt(op2) && getConstantOperandImmediateInt(op2) > 0;
    compileBinaryArithOpSlowCase(currentInstruction, op_mul, iter, result, op1, op2, types, op1HasImmediateIntFastCase, op2HasImmediateIntFastCase);
}

void JIT::emit_op_div(Instruction* currentInstruction)
{
    int dst = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);

    if (isOperandConstantImmediateDouble(op1)) {
        emitGetVirtualRegister(op1, regT0);
        add64(tagTypeNumberRegister, regT0);
        move64ToDouble(regT0, fpRegT0);
    } else if (isOperandConstantImmediateInt(op1)) {
        emitLoadInt32ToDouble(op1, fpRegT0);
    } else {
        emitGetVirtualRegister(op1, regT0);
        if (!types.first().definitelyIsNumber())
            emitJumpSlowCaseIfNotImmediateNumber(regT0);
        Jump notInt = emitJumpIfNotImmediateInteger(regT0);
        convertInt32ToDouble(regT0, fpRegT0);
        Jump skipDoubleLoad = jump();
        notInt.link(this);
        add64(tagTypeNumberRegister, regT0);
        move64ToDouble(regT0, fpRegT0);
        skipDoubleLoad.link(this);
    }

    if (isOperandConstantImmediateDouble(op2)) {
        emitGetVirtualRegister(op2, regT1);
        add64(tagTypeNumberRegister, regT1);
        move64ToDouble(regT1, fpRegT1);
    } else if (isOperandConstantImmediateInt(op2)) {
        emitLoadInt32ToDouble(op2, fpRegT1);
    } else {
        emitGetVirtualRegister(op2, regT1);
        if (!types.second().definitelyIsNumber())
            emitJumpSlowCaseIfNotImmediateNumber(regT1);
        Jump notInt = emitJumpIfNotImmediateInteger(regT1);
        convertInt32ToDouble(regT1, fpRegT1);
        Jump skipDoubleLoad = jump();
        notInt.link(this);
        add64(tagTypeNumberRegister, regT1);
        move64ToDouble(regT1, fpRegT1);
        skipDoubleLoad.link(this);
    }
    divDouble(fpRegT1, fpRegT0);
    
    // Is the result actually an integer? The DFG JIT would really like to know. If it's
    // not an integer, we increment a count. If this together with the slow case counter
    // are below threshold then the DFG JIT will compile this division with a specualtion
    // that the remainder is zero.
    
    // As well, there are cases where a double result here would cause an important field
    // in the heap to sometimes have doubles in it, resulting in double predictions getting
    // propagated to a use site where it might cause damage (such as the index to an array
    // access). So if we are DFG compiling anything in the program, we want this code to
    // ensure that it produces integers whenever possible.
    
    JumpList notInteger;
    branchConvertDoubleToInt32(fpRegT0, regT0, notInteger, fpRegT1);
    // If we've got an integer, we might as well make that the result of the division.
    emitFastArithReTagImmediate(regT0, regT0);
    Jump isInteger = jump();
    notInteger.link(this);
    moveDoubleTo64(fpRegT0, regT0);
    Jump doubleZero = branchTest64(Zero, regT0);
    add32(TrustedImm32(1), AbsoluteAddress(&m_codeBlock->addSpecialFastCaseProfile(m_bytecodeOffset)->m_counter));
    sub64(tagTypeNumberRegister, regT0);
    Jump trueDouble = jump();
    doubleZero.link(this);
    move(tagTypeNumberRegister, regT0);
    trueDouble.link(this);
    isInteger.link(this);

    emitPutVirtualRegister(dst, regT0);
}

void JIT::emitSlow_op_div(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);
    if (types.first().definitelyIsNumber() && types.second().definitelyIsNumber()) {
#ifndef NDEBUG
        breakpoint();
#endif
        return;
    }
    if (!isOperandConstantImmediateDouble(op1) && !isOperandConstantImmediateInt(op1)) {
        if (!types.first().definitelyIsNumber())
            linkSlowCase(iter);
    }
    if (!isOperandConstantImmediateDouble(op2) && !isOperandConstantImmediateInt(op2)) {
        if (!types.second().definitelyIsNumber())
            linkSlowCase(iter);
    }
    // There is an extra slow case for (op1 * -N) or (-N * op2), to check for 0 since this should produce a result of -0.
    JITSlowPathCall slowPathCall(this, currentInstruction, slow_path_div);
    slowPathCall.call();
}

void JIT::emit_op_sub(Instruction* currentInstruction)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);

    compileBinaryArithOp(op_sub, result, op1, op2, types);
    emitPutVirtualRegister(result);
}

void JIT::emitSlow_op_sub(Instruction* currentInstruction, Vector<SlowCaseEntry>::iterator& iter)
{
    int result = currentInstruction[1].u.operand;
    int op1 = currentInstruction[2].u.operand;
    int op2 = currentInstruction[3].u.operand;
    OperandTypes types = OperandTypes::fromInt(currentInstruction[4].u.operand);

    compileBinaryArithOpSlowCase(currentInstruction, op_sub, iter, result, op1, op2, types, false, false);
}

/* ------------------------------ END: OP_ADD, OP_SUB, OP_MUL ------------------------------ */

#endif // USE(JSVALUE64)

} // namespace JSC

#endif // ENABLE(JIT)
