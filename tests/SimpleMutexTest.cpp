/*
 * Copyright (c) 2014, YewMing Chen
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
#include "CppUTest/SimpleMutex.h"
#include "CppUTest/PlatformSpecificFunctions.h"

int mutexCreateCount = 0;
int mutexLockCount = 0;
int mutexUnlockCount = 0;
int mutexDestroyCount = 0;

PlatformSpecificMutex PlatformSpecificMutexCreate(void)
{
    mutexCreateCount++;
    return 0;
}

void PlatformSpecificMutexLock(PlatformSpecificMutex)
{
    mutexLockCount++;
}

void PlatformSpecificMutexUnlock(PlatformSpecificMutex)
{
    mutexUnlockCount++;
}

void PlatformSpecificMutexDestroy(PlatformSpecificMutex)
{
    mutexDestroyCount++;
}



TEST_GROUP(SimpleMutexTest)
{
    void setup()
    {
    }
    
    void teardown()
    {
    }
};

TEST(SimpleMutexTest, CreateAndDestroy)
{
    int tmpCreateCount = mutexCreateCount;
    int tmpDestroyCount = mutexDestroyCount;

    do {
        SimpleMutex mtx;
    } while (0);

    CHECK_EQUAL((tmpCreateCount + 1), mutexCreateCount);
    CHECK_EQUAL((tmpDestroyCount + 1), mutexDestroyCount);
}

TEST(SimpleMutexTest, LockUnlockTest)
{
    int tmpCreateCount = mutexCreateCount;
    int tmpLockCount = mutexLockCount;
    int tmpUnlockCount = mutexUnlockCount;
    int tmpDestroyCount = mutexDestroyCount;

    do {
        SimpleMutex mtx;
        mtx.Lock();
        mtx.Unlock();
    } while (0);
    
    CHECK_EQUAL((tmpCreateCount + 1), mutexCreateCount);
    CHECK_EQUAL((tmpLockCount + 1), mutexLockCount);
    CHECK_EQUAL((tmpUnlockCount + 1), mutexUnlockCount);
    CHECK_EQUAL((tmpDestroyCount + 1), mutexDestroyCount);
}
