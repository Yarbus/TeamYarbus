/*****************************************************************************/
/*!
\file   Serializer.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The JSON serializer will parse JSON data files and return values.
*/
/*****************************************************************************/

#include "Serializer/JSONSerializer.h"
#include <iostream>
#include "Common.h"
#include <cstdarg> //for variadic crap

namespace Framework
{
  namespace Serializer
  {
    ZeroSerializer::ZeroSerializer()
    {      
      //Create data type tree
      TypeList["int"]    = TYPE_INT;
      TypeList["uint"]   = TYPE_UINT;
      TypeList["long"]   = TYPE_LONG;
      TypeList["float"]  = TYPE_FLOAT;
      TypeList["double"] = TYPE_DOUBLE;
      TypeList["bool"]   = TYPE_BOOL;
      TypeList["string"] = TYPE_STRING;
      TypeList["Vec2"]   = TYPE_VEC2;
      TypeList["Vec3"]   = TYPE_VEC3;
      TypeList["Vec4"]   = TYPE_VEC4;
      TypeList["Quat"]   = TYPE_QUAT;
      TypeList["Enum"]   = TYPE_ENUM;

      trunk = NULL;
      CurrentNode = NULL;
      CurrentStem = NULL;
      inObject = 0;
      exitted = false;
    } //constructor JSONSerializer

    bool ZeroSerializer::open(const char* filepath)
    {
      if (dataFile.is_open())
      {
        dataFile.close();
      }

      string path("..//..//Resources//Levels//");
	  std::string extension(".data");

      path.append(filepath);
	  path.append(extension);
      dataFile.open(path);

      ErrorIf(dataFile.is_open() == false, "Could not open Level File");

      //Skip first 2 lines

      string dummy;
      std::getline(dataFile, dummy);
      std::getline(dataFile, dummy);

	  std::string dummy2;
	  dataFile >> dummy;
	  std::getline(dataFile, dummy2);
	  std::getline(dataFile, dummy2);
      ++inObject;
      trunk = AddNode(trunk, TYPE_OBJECT, dummy.c_str(), 0);
      CurrentNode = CurrentStem = trunk;
      return true;
      
      return false;
    } //function open

    void ZeroSerializer::close()
    {
      if (dataFile.is_open())
        dataFile.close();
    } //function close


    
    ZeroSerializer::~ZeroSerializer()
    {
      recursiveFree(trunk);
    }

    void ZeroSerializer::CreateArchive()
    {
      for (int it = 0; inObject > 0; ++it)
      {
        ReadLine();
        ParseLine();
      }
    }
    

    void ZeroSerializer::ReadLine()
    {
      if (dataFile.is_open())
        std::getline(dataFile, CurrentLine);
    }

    void ZeroSerializer::ParseLine()
    {
      string currentname;
      std::vector<string> tokens = Tokenize();
      if (CurrentLine.back() == ' ')
      {
        //Encountered new object
        currentname = tokens[0];
        if (!exitted) //Entering new object
        {
          ++inObject;
          CurrentNode->branch = AddNode(CurrentNode->branch, TYPE_OBJECT, currentname.c_str(), 0);
          CurrentNode->branch->previous = CurrentNode;
          CurrentStem = CurrentNode;
          CurrentNode = CurrentNode->branch;
          exitted = false;
        }
        else  //This is a new parallel object
        {
          ++inObject;
          CurrentNode->next = AddNode(CurrentNode->branch, TYPE_OBJECT, currentname.c_str(), 0);
          CurrentNode->next->previous = CurrentNode;
          CurrentNode = CurrentNode->next;
          exitted = false;
        }
        //Skip the next line
        string dummy;
        std::getline(dataFile, dummy);
      }
      //if the line ends in a ','
      else if (CurrentLine.back() == ',')
      {
        //if the object has ended
        if (CurrentLine[CurrentLine.length() - 2] == '}')
        {
          //Close object
          --inObject;
          if (prev != ' ')
          {
            CurrentStem = FindStem(CurrentNode);
            CurrentNode = CurrentStem;
            CurrentStem = FindStem(CurrentNode);
          }
          exitted = true;
        }
        //if it is a data field
        else
        {
          currentname = tokens[1];
          TYPE currenttype = DetermineType(&tokens[0]);
          if (CurrentNode->dataType == TYPE_OBJECT)
          {
            InterpretData(currenttype, &tokens, &(CurrentNode->branch));
            CurrentNode->branch->previous = CurrentNode;
            CurrentStem = CurrentNode;
            CurrentNode = CurrentNode->branch;
            CurrentNode->typeString = tokens[0];
          }
          else
          {
            InterpretData(currenttype, &tokens, &(CurrentNode->next));
            CurrentNode->next->previous = CurrentNode;
            CurrentNode = CurrentNode->next;
            CurrentNode->typeString = tokens[0];
          }
        }        
      }
      prev = CurrentLine.back();
    }

    void ZeroSerializer::InterpretData(TYPE currentdatatype, std::vector<string>* tokens, DataNode** node)
    {
      switch (currentdatatype)
      {
      case TYPE_UINT:
      {
        unsigned int number = std::stoul((*tokens)[3]);
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, number);
        break;
      }
      case TYPE_INT:
      {
        int number = std::stoi((*tokens)[3]);
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, number);
        break;
      }
      case TYPE_LONG:
      {
        long number = std::stol((*tokens)[3]);
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, number);
        break;
      }
      case TYPE_FLOAT:
      {
        float number = std::stof((*tokens)[3]);
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, number);
        break;
      }
      case TYPE_DOUBLE:
      {
        double number = std::stod((*tokens)[3]);
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, number);
        break;
      }
      case TYPE_BOOL:
      {
        bool state;
        if ((*tokens)[3].compare("true,") == 0)
          state = true;
        else
          state = false;
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, state);
        break;
      }
      case TYPE_STRING:
      {
        const char* currentname = (*tokens)[1].c_str();
        string str;
        unsigned int spos = CurrentLine.find_first_of("\"");
        unsigned int epos = CurrentLine.find_first_of("\"", spos + 1);
        for (unsigned int i = spos+1; i != epos; ++i)
          str.push_back(CurrentLine[i]);
        *node = AddNode(*node, currentdatatype, currentname, str);
        break;
      }
      case TYPE_VEC2:
      {
        std::vector<float> numarray;
        unsigned int spos = CurrentLine.find_first_of("[");
        unsigned int epos;
        for (int i = 0; i < 2; ++i)
        {
          string number;
          epos = CurrentLine.find_first_of(",]", spos);
          for (unsigned int j = spos+1; j != epos; ++j)
            number.push_back(CurrentLine[j]);
          numarray.push_back(std::stof(number));
          spos = epos + 1;
        }
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, numarray);
        break;
      }
      case TYPE_VEC3:
      {
        std::vector<float> numarray;
        unsigned int spos = CurrentLine.find_first_of("[");
        unsigned int epos;
        for (int i = 0; i < 3; ++i)
        {
          string number;
          epos = CurrentLine.find_first_of(",]", spos);
          for (unsigned int j = spos+1; j != epos; ++j)
            number.push_back(CurrentLine[j]);
          numarray.push_back(std::stof(number));
          spos = epos + 1;
        }
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, numarray);
        break;
      }
      case TYPE_VEC4:
      {
        std::vector<float> numarray;
        unsigned int spos = CurrentLine.find_first_of("[");
        unsigned int epos;
        for (int i = 0; i < 4; ++i)
        {
          string number;
          epos = CurrentLine.find_first_of(",]", spos);
          for (unsigned int j = spos+1; j != epos; ++j)
            number.push_back(CurrentLine[j]);
          numarray.push_back(std::stof(number));
          spos = epos + 1;
        }
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, numarray);
        break;
      }
      case TYPE_QUAT:
      {
        std::vector<float> quat;
        unsigned int spos = CurrentLine.find_first_of("[");
        unsigned int epos;
        for (int i = 0; i < 4; ++i)
        {
          string number;
          epos = CurrentLine.find_first_of(",]", spos);
          for (unsigned int j = spos + 1; j != epos; ++j)
            number.push_back(CurrentLine[j]);
          quat.push_back(std::stof(number));
          spos = epos + 1;
        }
        const char* currentname = (*tokens)[1].c_str();
        //Use the following function to get the z axis rotation
        //float ZRot = std::atan2(2 * (quat[0] * quat[1] + quat[3] * quat[2]), (1 - (2 * (quat[1] * quat[1] + quat[2] * quat[2]))));
        *node = AddNode(*node, currentdatatype, currentname, quat);
        break;
      }
      case TYPE_ENUM:
      {
        (*tokens)[3].pop_back();
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, (*tokens)[3]);
        break;
      }
      case TYPE_CUSTOM:
      {
        (*tokens)[3].pop_back();
        const char* currentname = (*tokens)[1].c_str();
        *node = AddNode(*node, currentdatatype, currentname, (*tokens)[3]);
        break;
      }
      } //switch

    }

    TYPE ZeroSerializer::DetermineType(string* typestring)
    {
      for (auto it = TypeList.begin(); it != TypeList.end(); ++it)
      {
        if (*typestring == it->first)
          return it->second;
      }
      return TYPE_CUSTOM;
    }

    /**************************************************************************/
    //Other helpers
    /**************************************************************************/
    //Split the line into seperate tokens
    std::vector<string> ZeroSerializer::Tokenize()
    {
      std::vector<string> tokens;
      std::size_t found = CurrentLine.find_first_not_of(" \t\n\"");
      std::size_t previous = found;

      unsigned int wordlength = CurrentLine.size();

      while (found != CurrentLine.npos)
      {
        found = CurrentLine.find_first_of(" \t\n\"", previous);
        string str;
        if (found >= wordlength)
          found = wordlength;
        for (unsigned int i = previous; i < found; ++i)
          str.push_back(CurrentLine[i]);
        previous = found;
        found = CurrentLine.find_first_not_of(" \t\n\"", previous);
        previous = found;
        if (str.size() > 0)
          tokens.push_back(str);
      }
      return tokens;
    }

    void ZeroSerializer::indent(int indentation)
    {
      for (int i = 0; i < indentation; ++i)
        std::cout << "\t";
    }

   
    

  } //namespace Serializer
} //namespace Framework