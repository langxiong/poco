//
// Utility.cpp
//
// $Id: //poco/Main/Data/PostgreSQL/src/Utility.cpp#5 $
//
// Library: Data
// Package: PostgreSQL
// Module:  Utility
//
// Implementation of Utility
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#include "Poco/Data/PostgreSQL/Utility.h"
#include "Poco/Data/PostgreSQL/SessionImpl.h"

#include "Poco/NumberFormatter.h"

namespace Poco {
namespace Data {
namespace PostgreSQL {


std::string
Utility::serverInfo( SessionHandle* aHandlePtr )
{
    std::string srvrInfo = "Process ID: ";
    
    srvrInfo.append( Poco::NumberFormatter::format( aHandlePtr->serverProcessID() ) );
    
    srvrInfo.append( " Protocol Version: ");
    
    srvrInfo.append( Poco::NumberFormatter::format( aHandlePtr->protocoVersion() ) );
    
	return srvrInfo;
}


std::string
Utility::serverInfo( Session& aSession )
{
	return serverInfo( handle( aSession ) );
}


int
Utility::serverVersion( SessionHandle* aHandlePtr )
{
	return aHandlePtr->serverVersion();
}


int
Utility::serverVersion( Session& aSession )
{
    return serverVersion( handle( aSession ) );
}


std::string
Utility::hostInfo( SessionHandle* aHandlePtr )
{
    SessionParametersMap parametersMap = aHandlePtr->connectionParameters();
    
    SessionParametersMap::const_iterator cItr = parametersMap.find( "host" );
    
    if ( parametersMap.end() == cItr )
    {
        return std::string();
    }
    
    return cItr->second.currentValue();
}


std::string
Utility::hostInfo( Session& aSession )
{
    return hostInfo( handle( aSession )  );
}


std::string
Utility::sessionEncoding( SessionHandle* aHandlePtr )
{
    return aHandlePtr->clientEncoding();
}


std::string
Utility::sessionEncoding( Poco::Data::Session& aSession )
{
    return sessionEncoding( handle( aSession )  );
}


} } } // namespace Poco::Data::PostgreSQL
