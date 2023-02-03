#include <sstream>

#include "StringUtilities.h"

using namespace std;
using namespace MegaManLofi;

vector<string> StringUtilities::Explode( const string& subject, char delimiter )
{
   vector<string> tokens;
   stringstream stream( subject );
   string token;

   while ( getline( stream, token, delimiter ) )
   {
      tokens.push_back( token );
   }

   return tokens;
}

vector<string> StringUtilities::ExplodeByWhitespace( const string& subject )
{
   vector<string> tokens;
   string currentToken = "";

   for ( int i = 0; i < (int)subject.length(); i++ )
   {
      auto c = subject[i];

      if ( c == ' ' || c == '\t' )
      {
         if ( currentToken.length() > 0 )
         {
            tokens.push_back( currentToken );
            currentToken = "";
         }
      }
      else
      {
         currentToken.push_back( c );
      }
   }

   if ( currentToken.length() > 0 )
   {
      tokens.push_back( currentToken );
   }

   return tokens;
}

string StringUtilities::ToUpper( const string& subject )
{
   string result = "";

   for ( int i = 0; i < (int)subject.length(); i++ )
   {
      result.push_back( toupper( subject[i] ) );
   }

   return result;
}
