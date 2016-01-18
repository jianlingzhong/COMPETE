/* ----------------------------------------------------------------------------
   libconfig - A structured configuration file parsing library
   Copyright (C) 2005  Mark A Lindner
 
   This file is part of libconfig.
    
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
    
   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
   ----------------------------------------------------------------------------
*/

#ifndef __libconfig_hpp
#define __libconfig_hpp

#include <stdio.h>
#include <string>
#include <map>

namespace libconfig
{

#include "libconfig.h"

  enum SettingType
  {
    TypeNone = 0,
    TypeGroup,
    TypeInt,
    TypeFloat,
    TypeString,
    TypeBoolean,
    TypeArray,
    TypeList
  };
  
  enum SettingFormat
  {
    FormatDefault = 0,
    FormatHex = 1
  };
  
  class LIBCONFIG_API ConfigException { };

  class LIBCONFIG_API SettingTypeException : public ConfigException { };

  class LIBCONFIG_API SettingNotFoundException : public ConfigException { };

  class LIBCONFIG_API SettingExistsException : public ConfigException { };

  class LIBCONFIG_API FileIOException : public ConfigException { };

  class LIBCONFIG_API ParseException : public ConfigException
  {
    friend class Config;
    
    private:

    int _line;
    const char *_error;

    ParseException(int line, const char *error)
      : _line(line), _error(error) {}

    public:

    virtual ~ParseException() { }

    inline int getLine() throw() { return(_line); }
    inline const char *getError() throw() { return(_error); }
  };

  class LIBCONFIG_API Setting
  {
    friend class Config;

    private:

    config_setting_t *_setting;
    SettingType _type;
    SettingFormat _format;

    Setting(config_setting_t *setting);

    void assertType(SettingType type) const
      throw(SettingTypeException);
    static Setting & wrapSetting(config_setting_t *setting);

    Setting(const Setting& other); // not supported
    Setting& operator=(const Setting& other); // not supported
    
    public:

    virtual ~Setting() throw();
  
    inline SettingType getType() const throw() { return(_type); }

    inline SettingFormat getFormat() const throw() { return(_format); }
    void setFormat(SettingFormat format) throw();

    operator bool() const throw(SettingTypeException);
    operator long() const throw(SettingTypeException);
    operator unsigned long() const throw(SettingTypeException);
    operator int() const throw(SettingTypeException);
    operator unsigned int() const throw(SettingTypeException);
    operator double() const throw(SettingTypeException);
    operator float() const throw(SettingTypeException);
    operator const char *() const throw(SettingTypeException);
    operator std::string() const throw(SettingTypeException);

    bool operator=(bool const& value) throw(SettingTypeException);
    long operator=(long const& value) throw(SettingTypeException);
    long operator=(int const& value) throw(SettingTypeException);
    double operator=(double const& value) throw(SettingTypeException);
    double operator=(float const& value) throw(SettingTypeException);
    const char *operator=(const char *value) throw(SettingTypeException);
    const std::string &operator=(const std::string & value)
      throw(SettingTypeException);    

    Setting & operator[](const char * key) const
      throw(SettingTypeException, SettingNotFoundException);

    inline Setting & operator[](const std::string & key) const
      throw(SettingTypeException, SettingNotFoundException)
    { return(operator[](key.c_str())); }

    Setting & operator[](int index) const
      throw(SettingTypeException, SettingNotFoundException);

    bool lookupValue(const char *name, bool &value) const throw();
    bool lookupValue(const char *name, long &value) const throw();
    bool lookupValue(const char *name, unsigned long &value) const throw();
    bool lookupValue(const char *name, int &value) const throw();
    bool lookupValue(const char *name, unsigned int &value) const throw();
    bool lookupValue(const char *name, double &value) const throw();
    bool lookupValue(const char *name, float &value) const throw();
    bool lookupValue(const char *name, const char *&value) const throw();

    inline bool lookupValue(const std::string &name, bool &value)
      const throw()
    { return(lookupValue(name.c_str(), value)); }
      
    inline bool lookupValue(const std::string &name, long &value) const throw()
    { return(lookupValue(name.c_str(), value)); }

    inline bool lookupValue(const std::string &name, unsigned long &value)
      const throw()
    { return(lookupValue(name.c_str(), value)); }

    inline bool lookupValue(const std::string &name, int &value) const throw()
    { return(lookupValue(name.c_str(), value)); }

    inline bool lookupValue(const std::string &name, unsigned int &value)
      const throw()
    { return(lookupValue(name.c_str(), value)); }

    inline bool lookupValue(const std::string &name, double &value) const
      throw()
    { return(lookupValue(name.c_str(), value)); }

    inline bool lookupValue(const std::string &name, float &value) const
      throw()
    { return(lookupValue(name.c_str(), value)); }

    inline bool lookupValue(const std::string &name, const char *&value) const
      throw()
    { return(lookupValue(name.c_str(), value)); }

    void remove(const char *name)
      throw(SettingTypeException, SettingNotFoundException);

    inline void remove(const std::string & name)
      throw(SettingTypeException, SettingNotFoundException)
    { remove(name.c_str()); }

    inline Setting & add(const std::string & name, SettingType type)
      throw(SettingTypeException, SettingExistsException)
    { return(add(name.c_str(), type)); }
  
    Setting & add(const char *name, SettingType type)
      throw(SettingTypeException, SettingExistsException);

    Setting & add(SettingType type)
      throw(SettingTypeException);

    inline bool exists(const std::string & name) const throw()
    { return(exists(name.c_str())); }

    bool exists(const char *name) const throw();
  
    int getLength() const throw();
    const char *getName() const throw();

    /*
    bool operator==(const Setting& other) const;

    inline bool operator!=(const Setting& other) const
    { return(! operator==(other)); }
    */
  };

  class LIBCONFIG_API Config
  {
    private:
    
    config_t _config;
    
    static void ConfigDestructor(void *arg);
    Config(const Config& other); // not supported
    Config& operator=(const Config& other); // not supported

    public:

    Config();
    virtual ~Config();

    void read(FILE *stream) throw(ParseException);
    void write(FILE *stream) const;

    void readFile(const char *filename) throw(FileIOException, ParseException);
    void writeFile(const char *filename) throw(FileIOException);

    inline Setting & lookup(const std::string &path) const
      throw(SettingNotFoundException)
    { return(lookup(path.c_str())); }
    
    Setting & lookup(const char *path) const
      throw(SettingNotFoundException);

    inline bool exists(const std::string & path) const throw()
    { return(exists(path.c_str())); }

    bool exists(const char *path) const throw();
    
    bool lookupValue(const char *path, bool &value) const throw();
    bool lookupValue(const char *path, long &value) const throw();
    bool lookupValue(const char *path, unsigned long &value) const throw();
    bool lookupValue(const char *path, int &value) const throw();
    bool lookupValue(const char *path, unsigned int &value) const throw();
    bool lookupValue(const char *path, double &value) const throw();
    bool lookupValue(const char *path, float &value) const throw();
    bool lookupValue(const char *path, const char *&value) const throw();

    inline bool lookupValue(const std::string &path, bool &value)
      const throw()
    { return(lookupValue(path.c_str(), value)); }
      
    inline bool lookupValue(const std::string &path, long &value) const throw()
    { return(lookupValue(path.c_str(), value)); }

    inline bool lookupValue(const std::string &path, unsigned long &value)
      const throw()
    { return(lookupValue(path.c_str(), value)); }

    inline bool lookupValue(const std::string &path, int &value) const throw()
    { return(lookupValue(path.c_str(), value)); }

    inline bool lookupValue(const std::string &path, unsigned int &value)
      const throw()
    { return(lookupValue(path.c_str(), value)); }

    inline bool lookupValue(const std::string &path, double &value) const
      throw()
    { return(lookupValue(path.c_str(), value)); }

    inline bool lookupValue(const std::string &path, float &value) const
      throw()
    { return(lookupValue(path.c_str(), value)); }

    inline bool lookupValue(const std::string &path, const char *&value) const
      throw()
    { return(lookupValue(path.c_str(), value)); }
    
    Setting & getRoot() const;    
  };

} // namespace libconfig

#endif // __libconfig_hpp
