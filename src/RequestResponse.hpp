#ifndef REQUEST_RESPONSE_H
#define REQUEST_RESPONSE_H 1

#include <event2/http.h>

#include <string>

class RequestResponse
{
public:
    RequestResponse(char const * const uri_path, struct evhttp_request * const req);
    ~RequestResponse();

    char const * GetUriPath() const
    {
        return m_UriPath;
    }

    void UseInputHeader();
    void UseOutputHeader();

    bool AddOutputHeaderByRef(std::string const & key, std::string const & value);
    bool AddOutputHeader(std::string const key, std::string const value);

    bool RemoveOutputHeaderByRef(std::string const & key);
    bool RemoveOutputHeader(std::string const key);

    bool GetInputHeaderByRef(std::string const & key, std::string & value);
    bool GetInputHeader(std::string const key, std::string value);

private:
    char const * const m_UriPath;
    struct evhttp_request * const m_Request;
    struct evkeyvalq * m_InputHeader;
    struct evkeyvalq * m_OutputHeader;
};

#endif
