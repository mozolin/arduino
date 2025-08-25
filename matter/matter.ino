/*   MATTER TEST   */
#include "Matter.h"
#include <app/server/OnboardingCodesUtil.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

using namespace chip;
using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

// Пин, к которому подключено реле
const int RELAY_GPIO = 13;

// Кластер и идентификатор атрибута, используемые устройством освещения Matter
const uint32_t CLUSTER_ID = OnOff::Id;
const uint32_t ATTRIBUTE_ID = OnOff::Attributes::OnOff::Id;

// Идентификатор и атрибут, которые будут назначены устройству Matter
uint16_t light_endpoint_id = 0;
attribute_t *attribute_ref;

// Callback функция для прослушивания различных событий устройства, связанных, например, с процессом настройки. Оставлена пустыой для простоты
static void on_device_event(const ChipDeviceEvent *event, intptr_t arg) {}

// Обработчик запросов на обновление атрибута.
// При запросе на обновление проверяется путь (кластер, атрибут)
// Если он соответствует атрибуту света, реле меняет свое состояние на новое.
static esp_err_t on_attribute_update(attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id,
                                     uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data) 
{
    if (type == attribute::PRE_UPDATE && endpoint_id == light_endpoint_id &&
        cluster_id == CLUSTER_ID && attribute_id == ATTRIBUTE_ID) 
    {
        // Мы получили запрос на обновление атрибута света вкл/выкл!
        bool new_state = val->val.b;
        digitalWrite(RELAY_GPIO, new_state);
    }

    return ESP_OK;
}

void setup() 
{
    Serial.begin(115200);
    pinMode(RELAY_GPIO, OUTPUT);

    // Включение отладочных сообщений
    esp_log_level_set("*", ESP_LOG_DEBUG);

    // Настройка узла Matter
    node::config_t node_config;
    node_t *node = node::create(&node_config, on_attribute_update, nullptr);

    // Настройка кластера/атрибутов света Matter с значениями по умолчанию
    on_off_light::config_t light_config;
    light_config.on_off.on_off = false;
    light_config.on_off.lighting.start_up_on_off = false;
    endpoint_t *endpoint = on_off_light::create(node, &light_config, ENDPOINT_FLAG_NONE, nullptr);

    // Сохранение ссылки на атрибут on/off. Он будет использован для чтения значения атрибута позже.
    attribute_ref = attribute::get(cluster::get(endpoint, CLUSTER_ID), ATTRIBUTE_ID);

    // Сохранение сгенерированного идентификатора конечной точки
    light_endpoint_id = endpoint::get_id(endpoint);

    // Настройка DAC (это хорошее место также для установки пользовательских данных, паролей и т. д.)
    esp_matter::set_custom_dac_provider(chip::Credentials::Examples::GetExampleDACProvider());

    // Запуск устройства Matter
    esp_matter::start(on_device_event);

    // Печать кодов, необходимых для настройки устройства Matter
    PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
}

void loop() 
{
    // Оставляем функций пустой
}