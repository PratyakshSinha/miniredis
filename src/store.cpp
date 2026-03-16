#include "store.h"
#include <fstream>

void Store::set(const std::string &key, const std::string &value, std::optional<int> ttlSeconds)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data[key] = value;
    if(ttlSeconds.has_value()) {
        m_expiry[key] = std::chrono::steady_clock::now() + std::chrono::seconds(ttlSeconds.value());
    } else {
        m_expiry.erase(key);
    }
}

std::optional<std::string> Store::get(const std::string& key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(!m_data.count(key) || isExpired(key)){
        return std::nullopt;
    }

    return std::optional<std::string>{m_data[key]};
}

bool Store::del(const std::string &key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_data.count(key) && !isExpired(key)){
        m_data.erase(key);
        m_expiry.erase(key);
        return true;
    }

    return false;
}

bool Store::exists(const std::string &key)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data.count(key) && !isExpired(key);
}

bool Store::save(const std::string &filename)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream outFile(filename);
    if(!outFile.is_open()){
        return false;
    }

    for(auto& it : m_data) {

        if(isExpired(it.first)) {
            continue;
        }

        if(m_expiry.count(it.first)){
            auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(m_expiry[it.first].time_since_epoch()).count();
            outFile << it.first << " " << it.second << " " << timestamp << "\n";
        } else {
            outFile << it.first << " " << it.second << " " << "0" << "\n";
        }
    }

    return true;
}

bool Store::load(const std::string &filename)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ifstream inFile(filename);
    if(!inFile.is_open()) {
        return false;
    }

    std::string key, value;
    int64_t expiry;

    while(inFile >> key >> value >> expiry){
        if(expiry > 0) {
            TimePoint tp = TimePoint(std::chrono::seconds(expiry));
            if(tp <= std::chrono::steady_clock::now()){
                continue;
            }
            m_data[key] = value;
            m_expiry[key] = tp;
        } else {
            m_data[key] = value;
        }
    }

    return true;
}

bool Store::isExpired(const std::string &key)
{
    if(!m_expiry.count(key)){
        return false;
    }

    if(std::chrono::steady_clock::now() >= m_expiry[key]){
        m_data.erase(key);
        m_expiry.erase(key);
        return true;
    }

    return false;
}

