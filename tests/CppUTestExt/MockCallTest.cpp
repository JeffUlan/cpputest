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

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "MockFailureTest.h"

TEST_GROUP(MockCallTest)
{
  void teardown()
  {
    mock().checkExpectations();
  }
};

TEST(MockCallTest, clear)
{
    mock().expectOneCall("func");
    mock().clear();
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockCallTest, checkExpectationsDoesntFail)
{
    mock().checkExpectations();
}

TEST(MockCallTest, exceptACallThatHappens)
{
    mock().expectOneCall("func");
    mock().actualCall("func");
    CHECK(! mock().expectedCallsLeft());
}

TEST(MockCallTest, exceptACallInceasesExpectedCallsLeft)
{
    mock().expectOneCall("func");
    CHECK(mock().expectedCallsLeft());
    mock().clear();
}

TEST(MockCallTest, checkExpectationsClearsTheExpectations)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foobar");
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock().expectOneCall("foobar");
    mock().checkExpectations();

    CHECK(! mock().expectedCallsLeft());
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, unexpectedCallHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest emptyExpectations;
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "func", emptyExpectations);

    mock().actualCall("func");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, expectMultipleCallsThatHappen)
{
    mock().expectOneCall("foo");
    mock().expectOneCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");
    mock().checkExpectations();
}

TEST(MockCallTest, expectOneCallHoweverMultipleHappened)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->callWasMade(1);
    expectations.addFunction("foo")->callWasMade(2);
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", expectations);

    mock().expectOneCall("foo");
    mock().expectOneCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");
    mock().actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherCallsExceptForTheExpectedOne)
{
    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().actualCall("bar").withParameter("foo", 1);;

    mock().clear();
}

TEST(MockCallTest, ignoreOtherCallsDoesntIgnoreMultipleCallsOfTheSameFunction)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->callWasMade(1);
    MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", expectations);

    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().actualCall("bar");
    mock().actualCall("foo");
    mock().actualCall("foo");

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo");
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock().expectOneCall("foo");
    mock().ignoreOtherCalls();
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCallTest, threeExpectedAndActual)
{
    mock().expectOneCall("function1");
    mock().expectOneCall("function2");
    mock().expectOneCall("function3");
    mock().actualCall("function1");
    mock().actualCall("function2");
    mock().actualCall("function3");

    mock().checkExpectations();
}



