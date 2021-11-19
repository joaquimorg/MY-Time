#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <array>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include "nrf52.h"

class Notification
{
    protected:

        const char * notification_names_def[11] = {
            "Notification", 
            "Missed Call", 
            "SMS", 
            "Social", 
            "e-Mail", 
            "Calendar", 
            "WhatsApp", 
            "Messenger", 
            "Instagram", 
            "Twitter", 
            "Skype"
        };

        struct _Notification {
            uint32_t notificationID;
            uint8_t type;
            uint16_t year;
            uint8_t month;
            uint8_t day;
            uint8_t hour;
            uint8_t minute;
            uint32_t timestamp;
            std::array<char, 31> subject;
            std::array<char, 61> message;
        };
        uint8_t notification_count = 0;
        uint8_t notification_index = 0;

        static constexpr uint8_t TotalNotifications = 5;
        std::array<_Notification, TotalNotifications> notifications;

    public:
        Notification(void) {}

        void add_notification(uint32_t notificationID, uint8_t type, uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, const char * subject, const char * message) {
            notification_count++;
            if (notification_count > TotalNotifications) {
                for (uint8_t i = 1; i < TotalNotifications; i++) {
                    notifications[i - 1] = notifications[i];
                }
                notification_count = TotalNotifications;
            }
            notifications[notification_count].notificationID = notificationID;
            notifications[notification_count].type = type;
            notifications[notification_count].timestamp = timestamp;
            notifications[notification_count].year = year;
            notifications[notification_count].month = month;
            notifications[notification_count].day = day;
            notifications[notification_count].hour = hour;
            notifications[notification_count].minute = minute;

            std::memcpy (notifications[notification_count].subject.data(), subject, 31);
            std::memcpy (notifications[notification_count].message.data(), message, 61);

            set_notification_index(notification_count);

        }

        uint8_t get_notification_count(void) { return notification_count; }
        uint32_t get_notification_id(uint8_t index) { return notifications[index].notificationID; }
        const char * get_notification_type(uint8_t index) { return notification_names_def[notifications[index].type]; }
        uint32_t get_notification_timestamp(uint8_t index) { return notifications[index].timestamp; }
        uint16_t get_notification_year(uint8_t index) { return notifications[index].year; }
        uint8_t get_notification_month(uint8_t index) { return notifications[index].month; }
        uint8_t get_notification_day(uint8_t index) { return notifications[index].day; }
        uint8_t get_notification_hour(uint8_t index) { return notifications[index].hour; }
        uint8_t get_notification_minute(uint8_t index) { return notifications[index].minute; }
        const char * get_notification_subject(uint8_t index) { return notifications[index].subject.data(); }
        const char * get_notification_message(uint8_t index) { return notifications[index].message.data(); }
        
        void clear_notifications(void) {
            notification_count = 0;
            notification_index = 0;
        }

        void clear_notification(uint8_t index) {
            for (uint8_t i = index; i < notification_count; i++) {
                notifications[i] = notifications[i + 1];
            }
            notification_count--;
            if (notification_index > notification_count) {
                notification_index = notification_count;
            }
        }

        void clear_notification_by_id(uint32_t id) {
            for (uint8_t i = 0; i < notification_count; i++) {
                if (notifications[i].notificationID == id) {
                    clear_notification(i);
                    return;
                }
            }            
        }

        uint8_t get_notification_index(void) { return notification_index; }
        void set_notification_index(uint8_t index) { notification_index = index; }
};


#endif //NOTIFICATION_H