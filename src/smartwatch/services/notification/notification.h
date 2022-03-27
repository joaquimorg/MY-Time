#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <array> 
#include <vector> 
#include <cstring> 
#include <cstddef> 
#include <cstdint> 
#include "nrf52.h"

class Notification
{
    protected : const char * notification_names_def[11] = {
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
        std :: array <char,31> subject;
        std :: array <char,61> message;
    };
    uint8_t notification_index = 0;

    static constexpr uint8_t TotalNotifications = 4;
    std :: vector < _Notification > notifications;

    public : Notification(void) {}

    void add_notification(uint32_t notificationID, uint8_t type, uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, const char * subject, uint8_t sizeSubject, const char * message, uint8_t sizeMessage) {

        if (notifications.size() == TotalNotifications) {
            notifications.pop_back();
        }
        _Notification notification;
        notification.notificationID = notificationID;
        notification.type = type;
        notification.timestamp = timestamp;
        notification.year = year;
        notification.month = month;
        notification.day = day;
        notification.hour = hour;
        notification.minute = minute;

        std :: memcpy(notification.subject.data(), subject, sizeSubject + 1);
        std :: memcpy(notification.message.data(), message, sizeMessage + 1);

        //notifications.push_back(notification);
        notifications.insert(notifications.begin(), notification);
        set_notification_index(0);

    }

    uint8_t get_notification_count(void) {
        return notifications.size();
    }
    uint32_t get_notification_id(uint8_t index) {
        return notifications[index].notificationID;
    }
    const char * get_notification_type(uint8_t index) {
        return notification_names_def[notifications[index].type];
    }
    uint32_t get_notification_timestamp(uint8_t index) {
        return notifications[index].timestamp;
    }
    uint16_t get_notification_year(uint8_t index) {
        return notifications[index].year;
    }
    uint8_t get_notification_month(uint8_t index) {
        return notifications[index].month;
    }
    uint8_t get_notification_day(uint8_t index) {
        return notifications[index].day;
    }
    uint8_t get_notification_hour(uint8_t index) {
        return notifications[index].hour;
    }
    uint8_t get_notification_minute(uint8_t index) {
        return notifications[index].minute;
    }
    const char * get_notification_subject(uint8_t index) {
        return notifications[index]
            .subject
            .data();
    }
    const char * get_notification_message(uint8_t index) {
        return notifications[index]
            .message
            .data();
    }

    void clear_notifications(void) {
        notifications.clear();
        notification_index = 0;
    }

    void clear_notification(uint8_t index) {
        notifications.erase(notifications.begin() + index);

        if (notification_index > get_notification_count()) {
            notification_index = get_notification_count();
        }
    }

    void clear_notification_by_id(uint32_t id) {
        for (uint8_t i = 0; i < get_notification_count(); i++) {
            if (notifications[i].notificationID == id) {
                clear_notification(i);
                return;
            }
        }
    }

    uint8_t get_notification_index(void) {
        return notification_index;
    }
    void set_notification_index(uint8_t index) {
        notification_index = index;
    }
};

#endif //NOTIFICATION_H