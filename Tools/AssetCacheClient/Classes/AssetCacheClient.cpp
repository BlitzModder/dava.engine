/*==================================================================================
    Copyright (c) 2008, binaryzebra
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the binaryzebra nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=====================================================================================*/

#include "AssetCacheClient.h"

#include "AddRequest.h"
#include "GetRequest.h"

AssetCacheClient::AssetCacheClient()
:   exitCode(AssetCacheClientConstants::EXIT_OK)
    ,   activeRequest(nullptr)
{
    requests.push_back(new AddRequest());
    requests.push_back(new GetRequest());
}

AssetCacheClient::~AssetCacheClient()
{
    activeRequest = nullptr;
    
    for(auto r: requests)
    {
        delete r;
    }
    requests.clear();
}

bool AssetCacheClient::ParseCommandLine(int argc, char * argv[])
{
    if(argc > 1)
    {
        const char *command = argv[1];
        for(auto & r : requests)
        {
            if (r->options.GetCommand() == command)
            {
                activeRequest = r;
                auto commandLineIsOk = r->options.Parse(argc, argv, 2);
                if(commandLineIsOk)
                {
                    exitCode = activeRequest->CheckOptions();
                    break;
                }
            }
        }
        
        if(exitCode != AssetCacheClientConstants::EXIT_OK)
        {
            PrintUsage();
            return false;
        }
        return true;
    }
    else
    {
        PrintUsage();
        exitCode = AssetCacheClientConstants::EXIT_WRONG_COMMAND_LINE;
    }
    return false;
}


void AssetCacheClient::PrintUsage()
{
    printf("Usage: AssetCacheClient <command>\n");
    printf("\n Commands: ");

    auto count = requests.size();
    for(auto & r : requests)
    {
        printf("%s", r->options.GetCommand().c_str());
        if(count != 1)
        {
            printf(", ");
        }
        --count;
    }

    printf("\n\n");
    for(auto & r : requests)
    {
        r->options.PrintUsage();
        printf("\n");
    }
}

void AssetCacheClient::Process()
{
    if(nullptr != activeRequest)
    {
        exitCode = activeRequest->Process();
    }
    else
    {
        exitCode = AssetCacheClientConstants::EXIT_WRONG_COMMAND_LINE;
    }
}
