#pragma once

#include "config.h"
#include "log.h"
#include <mqtt/client.h>
#include <optional>
#include <vector>

class MQCli
{

  public:
    enum class Type
    {
        PUB,
        SUB
    };

    MQCli(Type type, const std::string& client_id) : client_{config::SERVER_ADDRESS, client_id}, type_{type} { init(); }
    ~MQCli()
    {
        if (client_.is_connected())
        {
            lg::info("\nDisconnecting...");
            client_.disconnect();
            lg::info("...OK\n");
        }
    }

    template <typename T>
    bool publish(const T& data)
    {
        if (not client_.is_connected())
        {
            lg::error("Not connected");
            return false;
        }
        try
        {
            auto pubmsg = mqtt::make_message(config::TOPIC, &data, sizeof(data));
            pubmsg->set_qos(config::QOS);
            client_.publish(pubmsg);

            return true;
        }
        catch (const mqtt::exception& exc)
        {
            lg::error("\n{}\n", exc.what());
        }
        return false;
    }

    template <typename T>
    std::optional<T> consume_message()
    {
        try
        {
            while (true)
            {
                mqtt::const_message_ptr msg;
                if (client_.try_consume_message_for(&msg, config::WAIT_FOR))
                {
                    auto blob = msg->get_payload_ref();

                    if (blob.length() != sizeof(T))
                        continue;

                    const T data = *reinterpret_cast<const T*>(blob.data());

                    return data;
                }
                else if (not client_.is_connected())
                {
                    lg::info("Lost connection\n");
                    while (!client_.is_connected())
                    {
                        std::this_thread::sleep_for(config::WAIT_FOR);
                    }
                    lg::info("Re-established connection\n");
                }
                else
                    return {};
            }
        }
        catch (const mqtt::exception& exc)
        {
            lg::error("{}\n", exc.what());
            return {std::nullopt};
        }
    }

    bool is_valid() const { return valid_; }

  private:
    void init()
    {
        try
        {
            lg::info("Connecting...\n");
            switch (type_)
            {
            case Type::PUB:
                pub_connect();
                break;
            case Type::SUB:
                sub_connect();
                break;
            default:
                lg::error("Unknown type");
                return;
            }

            lg::info("Connecting...OK\n");
            valid_ = true;
        }
        catch (const mqtt::exception& exc)
        {
            lg::error("{}\n", exc.what());
        }
    }

    void pub_connect()
    {
        mqtt::connect_options conn_opts;
        conn_opts.set_keep_alive_interval(20);
        conn_opts.set_clean_session(true);
        client_.connect(conn_opts);
    }

    void sub_connect()
    {
        using namespace std::chrono;

        mqtt::connect_options conn_opts;
        conn_opts.set_automatic_reconnect(2s, 30s);
        conn_opts.set_clean_session(false);
        auto rsp = client_.connect(conn_opts);
        if (!rsp.is_session_present())
        {
            lg::info("Subscribing to topics...");
            client_.subscribe(config::TOPIC, config::QOS);
            lg::info("OK\n");
        }
        else
        {
            lg::info("Session already present. Skipping subscribe.\n");
        }
    }

  private:
    mqtt::client client_;
    bool valid_{false};
    Type type_;
};
