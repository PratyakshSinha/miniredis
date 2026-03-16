#ifndef STORE_H
#define STORE_H

#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <optional>


class Store {

public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

    /**
     * @brief set: sets the value for a key in memory
     * @param key: The key for which value is being set
     * @param value: The value for that is being set for a key
     * @param ttlSeconds: Time to live in seconds for a key
     */
    void set(const std::string& key, const std::string& value, std::optional<int> ttlSeconds = std::nullopt);

    /**
     * @brief get: Returns value of a key if exists in the memory
     * @param key: The key for which value needs to be fetched
     * @return value for the given key
     */
    std::optional<std::string> get(const std::string& key);

    /**
     * @brief del: Deletes value for a key in the memory
     * @param key: The key who's value has to be deleted
     * @return true if the value existed and deleted
     */
    bool del(const std::string& key);

    /**
     * @brief exists: Checks if the key exists in memory
     * @param key: The key which has to be checked for
     * @return true if the key exists
     */
    bool exists(const std::string& key);

    /**
     * @brief save: Dumps everything to a file
     * @param filename: Name of the file to whcich data needs to be dumped
     * @return true if  saved successfully
     */
    bool save(const std::string& filename);

    /**
     * @brief load: Reads everything back from a file
     * @param filename: Name of the file that needs to be loaded into the memory
     * @return true if loaded successfully
     */
    bool load(const std::string& filename);


private:

    /**
     * @brief isExpired: Checks if the key is expired or not
     * @param key: The key which needs to be checked for
     * @return true if the key was expired
     */
    bool isExpired(const std::string& key);

    std::unordered_map<std::string, std::string> m_data;
    std::unordered_map<std::string, TimePoint> m_expiry;
    std::mutex m_mutex;
};

#endif
