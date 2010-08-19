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

#include "CppUTestExt/MockSupport.h"
extern "C" {
	#include "CppUTestExt/MockSupport_c.h"
}

static MockSupport* currentMockSupport = NULL;
static MockFunctionCall* currentCall = NULL;

class MockCFunctionComparatorNode : public MockNamedValueComparator
{
public:
	MockCFunctionComparatorNode(MockCFunctionComparatorNode* next, MockTypeEqualFunction_c equal, MockTypeValueToStringFunction_c toString)
		: next_(next), equal_(equal), toString_(toString) {}
	virtual ~MockCFunctionComparatorNode() {};

	virtual bool isEqual(void* object1, void* object2)
	{
		return (bool) equal_(object1, object2);
	}
	virtual SimpleString valueToString(void* object)
	{
		return SimpleString(toString_(object));
	}

	MockCFunctionComparatorNode* next_;
	MockTypeEqualFunction_c equal_;
	MockTypeValueToStringFunction_c toString_;
};

static MockCFunctionComparatorNode* comparatorList_ = NULL;

extern "C" {

MockFunctionCall_c* expectOneCall_c(const char* name);
MockFunctionCall_c* actualCall_c(const char* name);

MockFunctionCall_c* withIntParamaters_c(const char* name, int value);
MockFunctionCall_c* withDoubleParameters_c(const char* name, double value);
MockFunctionCall_c* withStringParameters_c(const char* name, const char* value);
MockFunctionCall_c* withPointerParameters_c(const char* name, void* value);
MockFunctionCall_c* withParameterOfType_c(const char* type, const char* name, void* value);


void installComparator_c (const char* typeName, MockTypeEqualFunction_c isEqual, MockTypeValueToStringFunction_c valueToString)
{
	comparatorList_ = new MockCFunctionComparatorNode(comparatorList_, isEqual, valueToString);
	currentMockSupport->installComparator(typeName, *comparatorList_);
}

void removeAllComparators_c()
{
	while (comparatorList_) {
		MockCFunctionComparatorNode *next = comparatorList_->next_;
		delete comparatorList_;
		comparatorList_ = next;
	}
	currentMockSupport->removeAllComparators();
}

static MockFunctionCall_c gFunctionCall = {
		withIntParamaters_c,
		withDoubleParameters_c,
		withStringParameters_c,
		withPointerParameters_c,
		withParameterOfType_c
};

static MockSupport_c gMockSupport = {
		expectOneCall_c,
		actualCall_c,
		installComparator_c,
		removeAllComparators_c
};

MockFunctionCall_c* withIntParamaters_c(const char* name, int value)
{
	currentCall = &currentCall->withParameter(name, value);
	return &gFunctionCall;
}

MockFunctionCall_c* withDoubleParameters_c(const char* name, double value)
{
	currentCall = &currentCall->withParameter(name, value);
	return &gFunctionCall;
}

MockFunctionCall_c* withStringParameters_c(const char* name, const char* value)
{
	currentCall = &currentCall->withParameter(name, value);
	return &gFunctionCall;
}

MockFunctionCall_c* withPointerParameters_c(const char* name, void* value)
{
	currentCall = &currentCall->withParameter(name, value);
	return &gFunctionCall;
}

MockFunctionCall_c* withParameterOfType_c(const char* type, const char* name, void* value)
{
	currentCall = &currentCall->withParameterOfType(type, name, value);
	return &gFunctionCall;
}


MockFunctionCall_c* expectOneCall_c(const char* name)
{
	currentCall = &currentMockSupport->expectOneCall(name);
	return &gFunctionCall;
}

MockFunctionCall_c* actualCall_c(const char* name)
{
	currentCall = &currentMockSupport->actualCall(name);
	return &gFunctionCall;
}

MockSupport_c* mock_c()
{
	currentMockSupport = &mock();
	return &gMockSupport;
}

}
