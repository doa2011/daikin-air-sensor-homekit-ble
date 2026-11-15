# 大金空气传感器：HomeKit + Home Assistant BLE Proxy

基于 [louliangsheng/daikin-air-sensor](https://github.com/louliangsheng/daikin-air-sensor) 的扩展分支，保留原有 BRY88AA151K、BRY88AB151K 和 BRY88AB151K-CH2O 传感器功能，并增加：

- **ESPHome Bluetooth Proxy**：把设备附近的 BLE 广播和 GATT 通信转发到 Home Assistant，多个设备可共同扩大 HA 蓝牙覆盖范围。
- **原生 Apple HomeKit**：通过 HAP-ESPHome 直接接入 Apple「家庭」App。
- **Home Assistant 原生 API**：保留 ESPHome API，传感器仍直接接入 HA。

> 这里使用的是 ESPHome Bluetooth Proxy，不是 Bluetooth Mesh。各设备通过 Wi-Fi 接入 HA，HA 会统一管理多个 Proxy。

## 1. 支持型号

| 配置文件 | 型号/传感器 |
|---|---|
| `daikin-bry88aa151k.yaml` | BRY88AA151K，PM2005 |
| `daikin-bry88ab151k.yaml` | BRY88AB151K，PM2105 |
| `daikin-bry88ab151k-ch2o.yaml` | BRY88AB151K，PM2105 + ZE08-CH2O |

## 2. 配置 Wi-Fi 和 HomeKit

1. 将 `secrets.yaml.example` 复制为 ESPHome 配置目录中的 `secrets.yaml`。
2. 修改以下项目：

```yaml
wifi_ssid: "你的 Wi-Fi 名称"
wifi_password: "你的 Wi-Fi 密码"
homekit_setup_code: "321-54-987"
```

HomeKit 配对码格式必须为 `XXX-XX-XXX`。请自行设置，不要沿用示例码，也不要把真实的 `secrets.yaml` 提交到 GitHub。

## 3. 编译和烧录

选择与设备硬件匹配的 YAML 文件编译。首次刷写按原项目说明进入 ESP32 下载模式并通过串口烧录；后续可通过 ESPHome OTA 更新。

本分支采用 ESP-IDF，并加载 HAP-ESPHome。Bluetooth Proxy 与 HomeKit 同时运行会增加 RAM/IRAM 占用，具体余量以 CI 编译结果及设备运行日志为准。为给 BLE 和 HomeKit 留出资源，配置中移除了原来的 Web Server；HA API、传感器、校准和指示灯功能保留。

## 4. Home Assistant 使用

设备连接 Wi-Fi 并接入 ESPHome Integration 后，会同时提供：

- 原有空气质量传感器实体；
- ESPHome 原生 API；
- Bluetooth Proxy。

在 HA 的蓝牙集成中，多个 ESPHome Proxy 会自动参与蓝牙设备发现和通信。建议扫描模式先使用 Auto。

## 5. Apple 家庭配对

设备启动后，在 ESPHome 日志中查找 HomeKit 配对提示，使用 Apple「家庭」App 添加配件，并输入 `secrets.yaml` 中设置的配对码。

本配置将温度、湿度、CO₂ 和 PM2.5 传感器映射到 HomeKit。TVOC/甲醛传感器继续通过 Home Assistant 提供；HAP-ESPHome 当前支持的传感器类型有限，因此未将它们强行映射为不匹配的 HomeKit 类型。

## 6. 兼容性说明

- HAP-ESPHome：<https://github.com/rednblkx/HAP-ESPHome>
- ESPHome Bluetooth Proxy：<https://esphome.io/components/bluetooth_proxy/>
- 原始项目：<https://github.com/louliangsheng/daikin-air-sensor>

原项目使用旧式 ESPHome 自定义组件接口。本分支的 CI 会验证固定版本下的三个配置；若升级 ESPHome，需要同步评估旧式 `platform: custom` / `custom_component` 接口和 HAP-ESPHome 的兼容性。
