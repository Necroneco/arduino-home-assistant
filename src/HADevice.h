#ifndef AHA_HADEVICE_H
#define AHA_HADEVICE_H

#include <Arduino.h>

class HASerializer;

/**
 * This class represents your device that's going to be registered in the Home Assistant devices registry.
 * Each entity (HABinarySensor, HASensor, etc.) that you use will be owned by this device.
 */
class HADevice
{
public:
    /**
     * Constructs HADevice without the unique ID.
     *
     * @note You will need to set the ID using HADevice::setUniqueId method. Otherwise none of the entities will work.
     */
    HADevice();

    /**
     * Constructs HADevice with the given unique ID (string).
     * Keep the unique ID short to save the memory.
     *
     * @param uniqueId String with the null terminator.
     */
    HADevice(const char* uniqueId);

    /**
     * Constructs HADevice using the given byte array as the unique ID.
     * It works in the same way as HADevice::setUniqueId method.
     *
     * @param uniqueId Bytes array that's going to be converted into the string.
     * @param length Number of bytes in the array.
     */
    HADevice(const byte* uniqueId, const uint16_t length);

    /**
     * Deletes HASerializer and the availability topic if the shared availability was enabled.
     */
    ~HADevice();

    /**
     * Returns pointer to the unique ID. It can be nullptr if the device has no ID assigned.
     */
    inline const char* getUniqueId() const
        { return _uniqueId; }

    /**
     * Returns the instance of the HASerializer used by the device.
     * This method is used by all entities to serialize device's representation.
     */
    inline const HASerializer* getSerializer() const
        { return _serializer; }

    /**
     * Returns true if the shared availability is enabled for the device.
     */
    inline bool isSharedAvailabilityEnabled() const
        { return _sharedAvailability; }

    /**
     * Returns true if the extended unique IDs feature is enabled for the device.
     */
    inline bool isExtendedUniqueIdsEnabled() const
        { return _extendedUniqueIds; }

    /**
     * Returns availability topic generated by the HADevice::enableSharedAvailability method.
     * It can be nullptr if the shared availability is not enabled.
     */
    inline const char* getAvailabilityTopic() const
        { return _availabilityTopic; }

    /**
     * Returns online/offline state of the device.
     */
    inline bool isAvailable() const
        { return _available; }

    /**
     * Enables the use of extended unique IDs for all registered device types.
     * The unique ID of each device type will be prefixed with the device's ID once enabled.
     */
    inline void enableExtendedUniqueIds()
        { _extendedUniqueIds = true; }

    /**
     * Sets unique ID of the device based on the given byte array.
     * Each byte is converted into a hex string representation, so the final length of the unique ID will be twice as given.
     *
     * @param uniqueId Bytes array that's going to be converted into the string.
     * @param length Number of bytes in the array.
     * @note The unique ID can be set only once (via constructor or using this method).
     */
    bool setUniqueId(const byte* uniqueId, const uint16_t length);

    /**
     * Sets the "manufacturer" property that's going to be displayed in the Home Assistant.
     *
     * @param manufacturer Any string. Keep it short to save the memory.
     */
    void setManufacturer(const char* manufacturer);

    /**
     * Sets the "model" property that's going to be displayed in the Home Assistant.
     *
     * @param model Any string. Keep it short to save the memory.
     */
    void setModel(const char* model);

    void setURL(const char* url);

    /**
     * Sets the "name" property that's going to be displayed in the Home Assistant.
     *
     * @param name Any string. Keep it short to save the memory.
     */
    void setName(const char* name);

    /**
     * Sets the "software version" property that's going to be displayed in the Home Assistant.
     *
     * @param softwareVersion Any string. Keep it short to save the memory.
     */
    void setSoftwareVersion(const char* softwareVersion);

    /**
     * Sets the "configuration URL" property that's going to be used by the Home Assistant.
     *
     * @param url Configuration URL to publish.
     */
    void setConfigurationUrl(const char* url);

    /**
     * Sets device's availability and publishes MQTT message on the availability topic.
     * If the device is not connected to an MQTT broker or the shared availability is not enabled then nothing happens.
     *
     * @param online Set to true if the device should be displayed as available in the HA panel.
     */
    void setAvailability(bool online);

    /**
     * Enables the shared availability feature.
     */
    bool enableSharedAvailability();

    /**
     * Enables MQTT LWT feature.
     * Please note that the shared availability needs to be enabled first.
     */
    void enableLastWill();

    /**
     * Publishes current availability of the device on the availability topic.
     * If the device is not connected to an MQTT broker or the shared availability is not enabled then nothing happens.
     * This method is called by the HAMqtt when the connection to an MQTT broker is acquired.
     */
    void publishAvailability() const;

private:
    /// The unique ID of the device. It can be a memory allocated by HADevice::setUniqueId method.
    const char* _uniqueId;

    /// Specifies whether HADevice class owns the _uniqueId pointer.
    bool _ownsUniqueId;

    /// JSON serializer of the HADevice class. It's allocated in the constructor.
    HASerializer* _serializer;

    /// The availability topic allocated by HADevice::enableSharedAvailability method.
    char* _availabilityTopic;

    /// Specifies whether the shared availability is enabled.
    bool _sharedAvailability;

    /// Specifies whether the device is available (online / offline).
    bool _available;

    /// Specifies whether extended unique IDs feature is enabled.
    bool _extendedUniqueIds;
};

#endif
