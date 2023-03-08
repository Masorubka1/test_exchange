#include <boost/system/detail/errc.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <string>

#include "common/Common.hpp"
#include "common/json.hpp"
#include "common/order.hpp"
#include "server_common/timer.hpp"


using boost::asio::ip::tcp;

// Отправка сообщения на сервер по шаблону.
void SendMessage(
    tcp::socket& aSocket,
    const std::string& aId,
    const std::string& aRequestType,
    const nlohmann::json& aMessage)
{
    nlohmann::json req;
    req["UserId"] = aId;
    req["ReqType"] = aRequestType;
    req["Message"] = aMessage;

    std::cout << "here1: " << req.dump() << "\n";
    std::string request = req.dump();
    boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));
}

// Возвращает строку с ответом сервера на последний запрос.
std::string ReadMessage(tcp::socket& aSocket)
{
    boost::asio::streambuf b;
    boost::asio::read_until(aSocket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

// "Создаём" пользователя, получаем его ID.
std::string ProcessRegistration(tcp::socket& aSocket)
{
    std::string name = "Andrey";
    std::cout << "Hello! Enter your name: " + name << "\n";
    //std::cin >> name;
    nlohmann::json data;
    data["name"] = name;
    data["hash_password"] = "1234";

    std::cout << data.dump() << "\n";
    // Для регистрации Id не нужен, заполним его нулём
    SendMessage(aSocket, "0", Requests::Registration, data);
    return ReadMessage(aSocket);
}

int main()
{
    try
    {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        // Мы предполагаем, что для идентификации пользователя будет использоваться ID.
        // Тут мы "регистрируем" пользователя - отправляем на сервер имя, а сервер возвращает нам ID.
        // Этот ID далее используется при отправке запросов.
        std::string my_id = ProcessRegistration(s);
        std::cout << my_id << "\n";
        int cnt = 0;
        while (cnt < 4)
        {
            ++cnt;
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Hello Request\n"
                         "2) Order request\n"
                         "3) Exit\n"
                         << std::endl;

            short menu_option_num = cnt;
            //std::cin >> menu_option_num;
            switch (menu_option_num)
            {
                case 1:
                {
                    // Для примера того, как может выглядить взаимодействие с сервером
                    // реализован один единственный метод - Hello.
                    // Этот метод получает от сервера приветствие с именем клиента,
                    // отправляя серверу id, полученный при регистрации.
                    nlohmann::json data = "{}"_json;
                    SendMessage(s, my_id, Requests::Hello, data);
                    std::cout << ReadMessage(s) << "\n";
                    break;
                }
                case 2:
                {
                    nlohmann::json data;
                    data["hash_client"] = "1234";
                    data["order_type"] = "Ask";
                    data["order_status"] = "GOT";
                    data["exchange_type"] = "SPOT";
                    data["timestamp_user"] = server_common::Timer::now();
                    data["price"] = double(10);
                    data["volume"] = double(10);
                    SendMessage(s, my_id, Requests::Transaction, data);
                    std::cout << ReadMessage(s) << "\n";
                    break;
                }
                case 3:
                {
                    nlohmann::json data;
                    data["hash_client"] = "1234";
                    data["order_type"] = "Bid";
                    data["order_status"] = "GOT";
                    data["exchange_type"] = "SPOT";
                    data["timestamp_user"] = server_common::Timer::now();
                    data["price"] = double(10);
                    data["volume"] = double(10);
                    SendMessage(s, my_id, Requests::Transaction, data);
                    std::cout << ReadMessage(s) << "\n";
                    break;    
                }
                case 4:
                {
                    exit(0);
                    break;
                }
                default:
                {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}