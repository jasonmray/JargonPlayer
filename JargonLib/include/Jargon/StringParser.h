#pragma once

#include "StringUtilities.h"
#include "NumberParsing.h"

#include <cctype>
#include <stack>
#include <string>


namespace Jargon{

	template<typename CharType>
	class StringParser{
		public:
			StringParser(){
				this->sourceString = "";
				this->sourceStringLength = 0;
				this->currentPosition = 0;
			}

			StringParser(const CharType* sourceString) {
				this->sourceString = sourceString;
				this->sourceStringLength = Jargon::StringUtilities::stringLength(sourceString);
				this->currentPosition = 0;
			}

			StringParser(const std::basic_string<CharType> & sourceString){
				this->sourceString = sourceString.c_str();
				this->sourceStringLength = sourceString.length();
				this->currentPosition = 0;
			}

			StringParser(const CharType * sourceString, size_t sourceStringLength){
				this->sourceString = sourceString;
				this->sourceStringLength = sourceStringLength;
				this->currentPosition = 0;
			}

			~StringParser(){
			}

			bool isAtEnd() const{
				return currentPosition == sourceStringLength;
			}

			bool pushState(){
				stateStack.push(currentPosition);
				return true;
			}

			bool popState(){
				if( stateStack.empty() ){
					return false;
				}
				currentPosition = stateStack.top();
				stateStack.pop();
				return true;
			}

			bool peekChar(CharType *c){
				if( currentPosition < sourceStringLength ){
					*c = sourceString[currentPosition];
					return true;
				}
				return false;
			}

			bool lookAheadFor(CharType c){
				size_t lookAheadPosition = currentPosition;

				while(lookAheadPosition < sourceStringLength){
					if( sourceString[lookAheadPosition] == c ){
						return true;
					}
					lookAheadPosition++;
				}

				return false;
			}

			bool lookAheadFor(CharType c, size_t *distanceOut){
				size_t lookAheadPosition = currentPosition;

				while(lookAheadPosition < sourceStringLength){
					if( sourceString[lookAheadPosition] == c ){
						*distanceOut = lookAheadPosition - currentPosition;
						return true;
					}
					lookAheadPosition++;
				}

				return false;
			}

			bool readCharLiteral(CharType c){
				if( currentPosition < sourceStringLength && sourceString[currentPosition] == c ){
					currentPosition++;
					return true;
				}

				return false;
			}

			bool readStringLiteral(const CharType * s){
				size_t literalLength;
				literalLength = Jargon::StringUtilities::stringLength(s);

				if(
					((currentPosition + literalLength) <= sourceStringLength) &&
					Jargon::StringUtilities::stringNCompare(sourceString + currentPosition, s, literalLength) == 0
				){
					currentPosition += literalLength;
					return true;
				}

				return false;
			}

			bool readAnyOf(const CharType * chars, CharType * charOut){
				if( currentPosition < sourceStringLength ){
					CharType c;
					c = sourceString[currentPosition];

					if( isOneOf(c, chars) ){
						*charOut = c;
						currentPosition++;
						return true;
					}
				}
				return false;
			}

			bool readDigit(CharType * digitOut){
				if( currentPosition < sourceStringLength && std::isdigit(sourceString[currentPosition]) ){
					*digitOut = sourceString[currentPosition];
					currentPosition++;
					return true;
				}

				return false;
			}

			bool readWhitespace(CharType * charOut){
				if( currentPosition < sourceStringLength && std::isspace(sourceString[currentPosition]) ){
					*charOut = sourceString[currentPosition];
					currentPosition++;
					return true;
				}

				return false;
			}

			bool readWhitespace(std::basic_string<CharType> * stringOut){
				if( currentPosition == sourceStringLength ){
					return false;
				}

				while( currentPosition < sourceStringLength && std::isspace(sourceString[currentPosition]) ){
					stringOut->push_back(sourceString[currentPosition]);
					currentPosition++;
				}
				return true;
			}

			bool readChar(CharType * charOut){
				if( currentPosition < sourceStringLength ){
					*charOut = sourceString[currentPosition];
					currentPosition++;
					return true;
				}
				return false;
			}

			bool readInteger(int * valueOut){
				const CharType * parseEnd;
				if( Jargon::NumberParsing::parse(&sourceString[currentPosition], valueOut, &parseEnd) ){
					currentPosition += (parseEnd - &sourceString[currentPosition]);
					return true;
				}

				return false;
			}

			bool readUnsignedInteger(unsigned int * valueOut){
				const CharType * parseEnd;
				if( Jargon::NumberParsing::parse(&sourceString[currentPosition], valueOut, &parseEnd) ){
					currentPosition += (parseEnd - &sourceString[currentPosition]);
					return true;
				}

				return false;
			}

			bool readDouble(double * valueOut){
				const CharType * parseEnd;
				if( Jargon::NumberParsing::parse(&sourceString[currentPosition], valueOut, &parseEnd) ){
					currentPosition += (parseEnd - &sourceString[currentPosition]);
					return true;
				}

				return false;
			}

			bool readUntilWhitespace(std::basic_string<CharType> * stringOut){
				if( currentPosition == sourceStringLength ){
					return false;
				}

				while( currentPosition < sourceStringLength && !std::isspace(sourceString[currentPosition]) ){
					stringOut->push_back(sourceString[currentPosition]);
					currentPosition++;
				}
				return true;
			}

			bool readUntil(CharType c, std::basic_string<CharType> * stringOut){
				if( currentPosition == sourceStringLength ){
					return false;
				}

				while( currentPosition < sourceStringLength && sourceString[currentPosition] != c ){
					stringOut->push_back(sourceString[currentPosition]);
					currentPosition++;
				}
				return true;
			}

			bool readUntilAnyOf(const CharType * chars, std::basic_string<CharType> * stringOut){
				if( currentPosition == sourceStringLength ){
					return false;
				}

				while( currentPosition < sourceStringLength && !isOneOf(sourceString[currentPosition], chars) ){
					stringOut->push_back(sourceString[currentPosition]);
					currentPosition++;
				}

				return true;
			}

			bool readUntilEnd(std::basic_string<CharType> * stringOut){
				if(currentPosition == sourceStringLength){
					return false;
				}
				stringOut->assign(&sourceString[currentPosition], sourceStringLength - currentPosition);
				currentPosition = sourceStringLength;
				return true;
			}

			size_t skipWhitespace(){
				size_t numSkipped = 0;
				while( currentPosition < sourceStringLength && std::isspace(sourceString[currentPosition]) ){
					numSkipped++;
					currentPosition++;
				}
				return numSkipped;
			}

		private:
			static bool isOneOf(CharType c, const CharType * chars){
				const CharType * currentTestChar = chars;

				while( *currentTestChar != 0 ){
					if( c == *currentTestChar ){
						return true;
					}
					currentTestChar++;
				}
				return false;
			}

			size_t currentPosition;
			size_t sourceStringLength;
			const CharType * sourceString;

			std::stack<size_t> stateStack;
	};

}

