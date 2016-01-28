/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/CppUTestConfig.h"
#include "CppUTest/Utest.h"
#include "CppUTest/UtestMacros.h"
#include "CppUTest/PlatformSpecificFunctions_c.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupport_c.h"

class MockFailureReporterTestTerminatorForInCOnlyCode : public TestTerminatorWithoutExceptions
{
public:
    MockFailureReporterTestTerminatorForInCOnlyCode(bool crashOnFailure) : crashOnFailure_(crashOnFailure)
    {
    }

    virtual void exitCurrentTest() const _override
    {
        if (crashOnFailure_)
            UT_CRASH();

        TestTerminatorWithoutExceptions::exitCurrentTest();
    } // LCOV_EXCL_LINE
    // LCOV_EXCL_START
    virtual ~MockFailureReporterTestTerminatorForInCOnlyCode()
    {
    }
    // LCOV_EXCL_STOP
private:
    bool crashOnFailure_;

};

class MockFailureReporterForInCOnlyCode : public MockFailureReporter
{
public:
    void failTest(const MockFailure& failure) _override
    {
        if (!getTestToFail()->hasFailed())
            getTestToFail()->failWith(failure, MockFailureReporterTestTerminatorForInCOnlyCode(crashOnFailure_));
    } // LCOV_EXCL_LINE

};

static MockSupport* currentMockSupport = NULL;
static MockExpectedCall* expectedCall = NULL;
static MockActualCall* actualCall = NULL;
static MockFailureReporterForInCOnlyCode failureReporterForC;

class MockCFunctionComparatorNode : public MockNamedValueComparator
{
public:
    MockCFunctionComparatorNode(MockCFunctionComparatorNode* next, MockTypeEqualFunction_c equal, MockTypeValueToStringFunction_c toString)
        : next_(next), equal_(equal), toString_(toString) {}
    virtual ~MockCFunctionComparatorNode() {}

    virtual bool isEqual(const void* object1, const void* object2) _override
    {
        return equal_(object1, object2) != 0;
    }
    virtual SimpleString valueToString(const void* object) _override
    {
        return SimpleString(toString_(object));
    }

    MockCFunctionComparatorNode* next_;
    MockTypeEqualFunction_c equal_;
    MockTypeValueToStringFunction_c toString_;
};

static MockCFunctionComparatorNode* comparatorList_ = NULL;

class MockCFunctionCopierNode : public MockNamedValueCopier
{
public:
    MockCFunctionCopierNode(MockCFunctionCopierNode* next, MockTypeCopyFunction_c copier)
        : next_(next), copier_(copier) {}
    virtual ~MockCFunctionCopierNode() {}

    virtual void copy(void* dst, const void* src) _override
    {
        copier_(dst, src);
    }

    MockCFunctionCopierNode* next_;
    MockTypeCopyFunction_c copier_;
};

static MockCFunctionCopierNode* copierList_ = NULL;

extern "C" {

void strictOrder_c();
MockExpectedCall_c* expectOneCall_c(const char* name);
void expectNoCall_c(const char* name);
MockExpectedCall_c* expectNCalls_c(const int number, const char* name);
MockActualCall_c* actualCall_c(const char* name);
void disable_c();
void enable_c();
void ignoreOtherCalls_c();
void setIntData_c(const char* name, int value);
void setDoubleData_c(const char* name, double value);
void setStringData_c(const char* name, const char* value);
void setPointerData_c(const char* name, void* value);
void setConstPointerData_c(const char* name, const void* value);
void setFunctionPointerData_c(const char* name, void (*value)());
void setDataObject_c(const char* name, const char* type, void* value);
MockValue_c getData_c(const char* name);
int hasReturnValue_c();

void checkExpectations_c();
int expectedCallsLeft_c();
void clear_c();
void crashOnFailure_c(unsigned shouldCrash);

MockExpectedCall_c* withIntParameters_c(const char* name, int value);
MockExpectedCall_c* withUnsignedIntParameters_c(const char* name, unsigned int value);
MockExpectedCall_c* withLongIntParameters_c(const char* name, long int value);
MockExpectedCall_c* withUnsignedLongIntParameters_c(const char* name, unsigned long int value);
MockExpectedCall_c* withDoubleParameters_c(const char* name, double value);
MockExpectedCall_c* withStringParameters_c(const char* name, const char* value);
MockExpectedCall_c* withPointerParameters_c(const char* name, void* value);
MockExpectedCall_c* withConstPointerParameters_c(const char* name, const void* value);
MockExpectedCall_c* withFunctionPointerParameters_c(const char* name, void (*value)());
MockExpectedCall_c* withMemoryBufferParameters_c(const char* name, const unsigned char* value, size_t size);
MockExpectedCall_c* withParameterOfType_c(const char* type, const char* name, const void* value);
MockExpectedCall_c* withOutputParameterReturning_c(const char* name, const void* value, size_t size);
MockExpectedCall_c* withOutputParameterOfTypeReturning_c(const char* type, const char* name, const void* value);
MockExpectedCall_c* ignoreOtherParameters_c(void);
MockExpectedCall_c* andReturnIntValue_c(int value);
MockExpectedCall_c* andReturnUnsignedIntValue_c(unsigned int value);
MockExpectedCall_c* andReturnLongIntValue_c(long int value);
MockExpectedCall_c* andReturnUnsignedLongIntValue_c(unsigned long int value);
MockExpectedCall_c* andReturnDoubleValue_c(double value);
MockExpectedCall_c* andReturnStringValue_c(const char* value);
MockExpectedCall_c* andReturnPointerValue_c(void* value);
MockExpectedCall_c* andReturnConstPointerValue_c(const void* value);
MockExpectedCall_c* andReturnFunctionPointerValue_c(void (*value)());
MockValue_c expectedReturnValue_c();

MockActualCall_c* withActualIntParameters_c(const char* name, int value);
MockActualCall_c* withActualUnsignedIntParameters_c(const char* name, unsigned int value);
MockActualCall_c* withActualLongIntParameters_c(const char* name, long int value);
MockActualCall_c* withActualUnsignedLongIntParameters_c(const char* name, unsigned long int value);
MockActualCall_c* withActualDoubleParameters_c(const char* name, double value);
MockActualCall_c* withActualStringParameters_c(const char* name, const char* value);
MockActualCall_c* withActualPointerParameters_c(const char* name, void* value);
MockActualCall_c* withActualConstPointerParameters_c(const char* name, const void* value);
MockActualCall_c* withActualFunctionPointerParameters_c(const char* name, void (*value)());
MockActualCall_c* withActualMemoryBufferParameters_c(const char* name, const unsigned char* value, size_t size);
MockActualCall_c* withActualParameterOfType_c(const char* type, const char* name, const void* value);
MockActualCall_c* withActualOutputParameter_c(const char* name, void* value);
MockActualCall_c* withActualOutputParameterOfType_c(const char* type, const char* name, void* value);
int hasActualReturnValue_c();
MockValue_c actualReturnValue_c();
int actualIntReturnValue_c();
int actualReturnIntValueOrDefault_c(int defaultValue);
unsigned int actualUnsignedIntReturnValue_c();
unsigned int actualReturnUnsignedIntValueOrDefault_c(unsigned int defaultValue);
long int actualLongIntReturnValue_c();
long int actualReturnLongIntValueOrDefault_c(long int defaultValue);
unsigned long int actualUnsignedLongIntReturnValue_c();
unsigned long int actualReturnUnsignedLongIntValueOrDefault_c(unsigned long int defaultValue);
const char* actualStringReturnValue_c(void);
const char* actualReturnStringValueOrDefault_c(const char * defaultValue);
double actualDoubleReturnValue_c(void);
double actualReturnDoubleValueOrDefault_c(double defaultValue);

static void installComparator_c (const char* typeName, MockTypeEqualFunction_c isEqual, MockTypeValueToStringFunction_c valueToString)
{
    comparatorList_ = new MockCFunctionComparatorNode(comparatorList_, isEqual, valueToString);
    currentMockSupport->installComparator(typeName, *comparatorList_);
}

static void installCopier_c (const char* typeName, MockTypeCopyFunction_c copier)
{
    copierList_ = new MockCFunctionCopierNode(copierList_, copier);
    currentMockSupport->installCopier(typeName, *copierList_);
}

static void removeAllComparatorsAndCopiers_c()
{
    while (comparatorList_) {
        MockCFunctionComparatorNode *next = comparatorList_->next_;
        delete comparatorList_;
        comparatorList_ = next;
    }
    while (copierList_) {
        MockCFunctionCopierNode *next = copierList_->next_;
        delete copierList_;
        copierList_ = next;
    }
    currentMockSupport->removeAllComparatorsAndCopiers();
}

static MockExpectedCall_c gExpectedCall = {
        withIntParameters_c,
        withUnsignedIntParameters_c,
        withLongIntParameters_c,
        withUnsignedLongIntParameters_c,
        withDoubleParameters_c,
        withStringParameters_c,
        withPointerParameters_c,
        withConstPointerParameters_c,
        withFunctionPointerParameters_c,
        withMemoryBufferParameters_c,
        withParameterOfType_c,
        withOutputParameterReturning_c,
        withOutputParameterOfTypeReturning_c,
        ignoreOtherParameters_c,
        andReturnUnsignedIntValue_c,
        andReturnIntValue_c,
        andReturnLongIntValue_c,
        andReturnUnsignedLongIntValue_c,
        andReturnDoubleValue_c,
        andReturnStringValue_c,
        andReturnPointerValue_c,
        andReturnConstPointerValue_c,
        andReturnFunctionPointerValue_c,
};

static MockActualCall_c gActualCall = {
        withActualIntParameters_c,
        withActualUnsignedIntParameters_c,
        withActualLongIntParameters_c,
        withActualUnsignedLongIntParameters_c,
        withActualDoubleParameters_c,
        withActualStringParameters_c,
        withActualPointerParameters_c,
        withActualConstPointerParameters_c,
        withActualFunctionPointerParameters_c,
        withActualMemoryBufferParameters_c,
        withActualParameterOfType_c,
        withActualOutputParameter_c,
        withActualOutputParameterOfType_c,
        hasActualReturnValue_c,
        actualReturnValue_c,
        actualIntReturnValue_c,
        actualReturnIntValueOrDefault_c,
        actualUnsignedIntReturnValue_c,
        actualReturnUnsignedIntValueOrDefault_c,
        actualLongIntReturnValue_c,
        actualReturnLongIntValueOrDefault_c,
        actualUnsignedLongIntReturnValue_c,
        actualReturnUnsignedLongIntValueOrDefault_c,
        actualStringReturnValue_c,
        actualReturnStringValueOrDefault_c,
        actualDoubleReturnValue_c,
        actualReturnDoubleValueOrDefault_c,
};

static MockSupport_c gMockSupport = {
        strictOrder_c,
        expectOneCall_c,
        expectNoCall_c,
        expectNCalls_c,
        actualCall_c,
        hasReturnValue_c,
        actualReturnValue_c,
        actualIntReturnValue_c,
        actualReturnIntValueOrDefault_c,
        actualUnsignedIntReturnValue_c,
        actualReturnUnsignedIntValueOrDefault_c,
        actualLongIntReturnValue_c,
        actualReturnLongIntValueOrDefault_c,
        actualUnsignedLongIntReturnValue_c,
        actualReturnUnsignedLongIntValueOrDefault_c,
        actualStringReturnValue_c,
        actualReturnStringValueOrDefault_c,
        actualDoubleReturnValue_c,
        actualReturnDoubleValueOrDefault_c,
        setIntData_c,
        setStringData_c,
        setDoubleData_c,
        setPointerData_c,
        setConstPointerData_c,
        setFunctionPointerData_c,
        setDataObject_c,
        getData_c,
        disable_c,
        enable_c,
        ignoreOtherCalls_c,
        checkExpectations_c,
        expectedCallsLeft_c,
        clear_c,
        crashOnFailure_c,
        installComparator_c,
        installCopier_c,
        removeAllComparatorsAndCopiers_c,
};

MockExpectedCall_c* withIntParameters_c(const char* name, int value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withUnsignedIntParameters_c(const char* name, unsigned int value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withLongIntParameters_c(const char* name, long int value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withUnsignedLongIntParameters_c(const char* name, unsigned long int value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withDoubleParameters_c(const char* name, double value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withStringParameters_c(const char* name, const char* value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withPointerParameters_c(const char* name, void* value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withConstPointerParameters_c(const char* name, const void* value)
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withFunctionPointerParameters_c(const char* name, void (*value)())
{
    expectedCall = &expectedCall->withParameter(name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withMemoryBufferParameters_c(const char* name, const unsigned char* value, size_t size)
{
    expectedCall = &expectedCall->withParameter(name, value, size);
    return &gExpectedCall;
}

MockExpectedCall_c* withParameterOfType_c(const char* type, const char* name, const void* value)
{
    expectedCall = &expectedCall->withParameterOfType(type, name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* withOutputParameterReturning_c(const char* name, const void* value, size_t size)
{
    expectedCall = &expectedCall->withOutputParameterReturning(name, value, size);
    return &gExpectedCall;
}

MockExpectedCall_c* withOutputParameterOfTypeReturning_c(const char* type, const char* name, const void* value)
{
    expectedCall = &expectedCall->withOutputParameterOfTypeReturning(type, name, value);
    return &gExpectedCall;
}

MockExpectedCall_c* ignoreOtherParameters_c()
{
    expectedCall = &expectedCall->ignoreOtherParameters();
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnUnsignedIntValue_c(unsigned int value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnIntValue_c(int value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnLongIntValue_c(long int value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnUnsignedLongIntValue_c(unsigned long int value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnDoubleValue_c(double value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnStringValue_c(const char* value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnPointerValue_c(void* value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnConstPointerValue_c(const void* value)
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

MockExpectedCall_c* andReturnFunctionPointerValue_c(void (*value)())
{
    expectedCall = &expectedCall->andReturnValue(value);
    return &gExpectedCall;
}

static MockValue_c getMockValueCFromNamedValue(const MockNamedValue& namedValue)
{
    MockValue_c returnValue;
    if (SimpleString::StrCmp(namedValue.getType().asCharString(), "int") == 0) {
        returnValue.type = MOCKVALUETYPE_INTEGER;
        returnValue.value.intValue = namedValue.getIntValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "unsigned int") == 0) {
        returnValue.type = MOCKVALUETYPE_UNSIGNED_INTEGER;
        returnValue.value.unsignedIntValue = namedValue.getUnsignedIntValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "long int") == 0) {
        returnValue.type = MOCKVALUETYPE_LONG_INTEGER;
        returnValue.value.longIntValue = namedValue.getLongIntValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "unsigned long int") == 0) {
        returnValue.type = MOCKVALUETYPE_UNSIGNED_LONG_INTEGER;
        returnValue.value.unsignedLongIntValue = namedValue.getUnsignedLongIntValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "double") == 0) {
        returnValue.type = MOCKVALUETYPE_DOUBLE;
        returnValue.value.doubleValue = namedValue.getDoubleValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "const char*") == 0) {
        returnValue.type = MOCKVALUETYPE_STRING;
        returnValue.value.stringValue = namedValue.getStringValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "void*") == 0) {
        returnValue.type = MOCKVALUETYPE_POINTER;
        returnValue.value.pointerValue = namedValue.getPointerValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "const void*") == 0) {
        returnValue.type = MOCKVALUETYPE_CONST_POINTER;
        returnValue.value.constPointerValue = namedValue.getConstPointerValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "void (*)()") == 0) {
        returnValue.type = MOCKVALUETYPE_FUNCTIONPOINTER;
        returnValue.value.functionPointerValue = namedValue.getFunctionPointerValue();
    }
    else if (SimpleString::StrCmp(namedValue.getType().asCharString(), "const unsigned char*") == 0) {
        returnValue.type = MOCKVALUETYPE_MEMORYBUFFER;
        returnValue.value.memoryBufferValue = namedValue.getMemoryBuffer();
    }
    else {
        returnValue.type = MOCKVALUETYPE_OBJECT;
        returnValue.value.objectValue = namedValue.getObjectPointer();
    }
    return returnValue;
}

void strictOrder_c(void)
{
    currentMockSupport->strictOrder();
}

MockExpectedCall_c* expectOneCall_c(const char* name)
{
    expectedCall = &currentMockSupport->expectOneCall(name);
    return &gExpectedCall;
}

void expectNoCall_c(const char* name)
{
    currentMockSupport->expectNoCall(name);
}

MockExpectedCall_c* expectNCalls_c(const int number, const char* name)
{
    expectedCall = &currentMockSupport->expectNCalls(number, name);
    return &gExpectedCall;
}

MockActualCall_c* actualCall_c(const char* name)
{
    actualCall = &currentMockSupport->actualCall(name);
    return &gActualCall;
}

MockActualCall_c* withActualIntParameters_c(const char* name, int value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualUnsignedIntParameters_c(const char* name, unsigned int value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualLongIntParameters_c(const char* name, long int value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualUnsignedLongIntParameters_c(const char* name, unsigned long int value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualDoubleParameters_c(const char* name, double value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualStringParameters_c(const char* name, const char* value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualPointerParameters_c(const char* name, void* value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualConstPointerParameters_c(const char* name, const void* value)
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualFunctionPointerParameters_c(const char* name, void (*value)())
{
    actualCall = &actualCall->withParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualMemoryBufferParameters_c(const char* name, const unsigned char* value, size_t size)
{
    actualCall = &actualCall->withParameter(name, value, size);
    return &gActualCall;
}

MockActualCall_c* withActualParameterOfType_c(const char* type, const char* name, const void* value)
{
    actualCall = &actualCall->withParameterOfType(type, name, value);
    return &gActualCall;
}

MockActualCall_c* withActualOutputParameter_c(const char* name, void* value)
{
    actualCall = &actualCall->withOutputParameter(name, value);
    return &gActualCall;
}

MockActualCall_c* withActualOutputParameterOfType_c(const char* type, const char* name, void* value)
{
    actualCall = &actualCall->withOutputParameterOfType(type, name, value);
    return &gActualCall;
}

int hasActualReturnValue_c(void)
{
    return actualCall->hasReturnValue();
}

MockValue_c actualReturnValue_c()
{
    return getMockValueCFromNamedValue(actualCall->returnValue());
}

int actualIntReturnValue_c()
{
    return getMockValueCFromNamedValue(actualCall->returnValue()).value.intValue;
}

int actualReturnIntValueOrDefault_c(int defaultValue)
{
    if (!hasActualReturnValue_c()) {
        return defaultValue;
    }
    return actualIntReturnValue_c();
}

unsigned int actualUnsignedIntReturnValue_c()
{
    return getMockValueCFromNamedValue(actualCall->returnValue()).value.unsignedIntValue;
}

unsigned int actualReturnUnsignedIntValueOrDefault_c(unsigned int defaultValue)
{
    if (!hasActualReturnValue_c()) {
        return defaultValue;
    }
    return actualUnsignedIntReturnValue_c();
}

long int actualLongIntReturnValue_c()
{
    return getMockValueCFromNamedValue(actualCall->returnValue()).value.longIntValue;
}

long int actualReturnLongIntValueOrDefault_c(long int defaultValue)
{
    if (!hasActualReturnValue_c()) {
        return defaultValue;
    }
    return actualLongIntReturnValue_c();
}

unsigned long int actualUnsignedLongIntReturnValue_c()
{
    return getMockValueCFromNamedValue(actualCall->returnValue()).value.unsignedLongIntValue;
}

unsigned long int actualReturnUnsignedLongIntValueOrDefault_c(unsigned long int defaultValue)
{
    if (!hasActualReturnValue_c()) {
        return defaultValue;
    }
    return actualUnsignedLongIntReturnValue_c();
}

const char* actualStringReturnValue_c(void)
{
    return getMockValueCFromNamedValue(actualCall->returnValue()).value.stringValue;
}

const char* actualReturnStringValueOrDefault_c(const char * defaultValue)
{
    if (!hasActualReturnValue_c()) {
        return defaultValue;
    }
    return actualStringReturnValue_c();
}

double actualDoubleReturnValue_c()
{
    return getMockValueCFromNamedValue(actualCall->returnValue()).value.doubleValue;
}

double actualReturnDoubleValueOrDefault_c(double defaultValue)
{
    if (!hasActualReturnValue_c()) {
        return defaultValue;
    }
    return actualDoubleReturnValue_c();
}

void disable_c(void)
{
    currentMockSupport->disable();
}

void enable_c(void)
{
    currentMockSupport->enable();
}

void ignoreOtherCalls_c(void)
{
    currentMockSupport->ignoreOtherCalls();
}

void setIntData_c(const char* name, int value)
{
    currentMockSupport->setData(name, value);
}

void setDoubleData_c(const char* name, double value)
{
    currentMockSupport->setData(name, value);
}

void setStringData_c(const char* name, const char* value)
{
    currentMockSupport->setData(name, value);
}

void setPointerData_c(const char* name, void* value)
{
    currentMockSupport->setData(name, value);
}

void setConstPointerData_c(const char* name, const void* value)
{
    currentMockSupport->setData(name, value);
}

void setFunctionPointerData_c(const char* name, void (*value)())
{
    currentMockSupport->setData(name, value);
}

void setDataObject_c(const char* name, const char* type, void* value)
{
    currentMockSupport->setDataObject(name, type, value);
}

MockValue_c getData_c(const char* name)
{
    return getMockValueCFromNamedValue(currentMockSupport->getData(name));
}

int hasReturnValue_c()
{
    return currentMockSupport->hasReturnValue();
}

void checkExpectations_c()
{
    currentMockSupport->checkExpectations();
}

int expectedCallsLeft_c()
{
    return currentMockSupport->expectedCallsLeft();
}

void clear_c()
{
    currentMockSupport->clear();
}

void crashOnFailure_c(unsigned shouldCrash)
{
    currentMockSupport->crashOnFailure(0 != shouldCrash);
}

MockSupport_c* mock_c()
{
    currentMockSupport = &mock("", &failureReporterForC);
    return &gMockSupport;
}

MockSupport_c* mock_scope_c(const char* scope)
{
    currentMockSupport = &mock(scope, &failureReporterForC);
    return &gMockSupport;
}

}
