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

#include "TestOutput.h"
#include <stdio.h>
#include "SimpleString.h"
#include "Utest.h"
#include "Failure.h"
#include "TestResult.h"

TestOutput::TestOutput() : dotCount_(0), verbose_(false)
{}

TestOutput::~TestOutput()
{}

void TestOutput::verbose()
{
	verbose_ = true;
}

void TestOutput::print(const char* s)
{
    for (const char* p = s; *p; p++)
        putchar(*p);
}

void TestOutput::print(long n)
{
  print(StringFrom(n).asCharString());
}

void TestOutput::printDouble(double d)
{
  print(StringFrom(d).asCharString());
}

void TestOutput::printHex(long n)
{
  print(HexStringFrom(n).asCharString());
}


TestOutput& operator<<(TestOutput& p, const char* s)
{
  p.print(s);
  return p;
}

TestOutput& operator<<(TestOutput& p, long int i)
{
  p.print(i);
  return p;
}

void TestOutput::printCurrentTest(const Utest& test)
{
	if (verbose_) {
		print(test.getFormattedName().asCharString());
		print("\n");
	}
	else {		
		print(".");
		if (++dotCount_ % 50 == 0)
			print("\n");
	}
}

void TestOutput::printTestsStarted()
{
}

void TestOutput::flush()
{
}

void TestOutput::printTotalExecutionTime(const TestResult& result)
{
	print("\n");
	print(result.getTestCount());
	print(" tests ran in ");	
	printDouble(result.getTotalExecutionTime());
	print(" seconds");	
}

void TestOutput::printTestsEnded(const TestResult& result)
{
	if (result.getFailureCount() > 0) {
	  print("\nErrors (");
      print(result.getFailureCount());
      print(" failures, ");
	}
	else {
		print("\nOK (");
	}
	print(result.getTestCount());
	print(" tests, ");
	print(result.getRunCount());
	print(" ran, ");
	print(result.getCheckCount());
	print(" checks, ");
	print(result.getIgnoredCount());
	print(" ignored, ");
	print(result.getFilteredOutCount());
	print(" filtered out)\n\n");
}


void TestOutput::printTestRun(int number, int total)
{
	if (total > 1) {
		print("Test run ");
		print(number);
		print(" of ");
		print(total);
		print("\n");
	}
}

void TestOutput::print(const Failure& failure)
{
    print("\n");
    print(failure.getFileName().asCharString());
    print(":");
    print(failure.getLineNumber());
    print(":");
    print(" error: ");    
    print("Failure in ");
    print(failure.getTestName().asCharString());
    print("\n");
    print("\t");
    print(failure.getMessage().asCharString());
    print("\n\n");
}
