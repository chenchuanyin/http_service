# ifndef _LOG_LIB_CPLUSPLUS_
# define _LOG_LIB_CPLUSPLUS_
# include <map>
# include <string>
namespace loglib
{
	void init(const std::string& configFile);
	void sendlog(const std::string& logtype, const std::map<std::string,std::string> & logmap);
	void close();
}
# endif
