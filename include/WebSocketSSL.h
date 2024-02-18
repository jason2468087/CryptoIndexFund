#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

namespace WebSocketSSL
{
    void on_message(websocketpp::connection_hdl, client::message_ptr msg);
    bool verify_subject_alternative_name(const char * hostname, X509 * cert);
    bool verify_common_name(char const * hostname, X509 * cert);
    bool verify_certificate(const char * hostname, bool preverified, boost::asio::ssl::verify_context& ctx);
    context_ptr on_tls_init(const char * hostname, websocketpp::connection_hdl);
    void startWS(std::string, void (*evtFuncPtr)(std::string,double));
}