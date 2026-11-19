# 大金空气传感器：HomeKit + Home Assistant BLE Proxy

基于 [louliangsheng/daikin-air-sensor](https://github.com/louliangsheng/daikin-air-sensor) 的扩展项目。保留三个原始硬件型号的传感器、校准、指示灯功能，并增加 Apple HomeKit 与 ESPHome Bluetooth Proxy。

## 支持型号

| 配置文件 | 硬件配置 |
|---|---|
| `daikin-bry88aa151k.yaml` | BRY88AA151K，PM2005 |
| `daikin-bry88ab151k.yaml` | BRY88AB151K，PM2105 |
| `daikin-bry88ab151k-ch2o.yaml` | BRY88AB151K，PM2105 + ZE08-CH2O |

## 功能

- Home Assistant：保留 ESPHome Native API，传感器实体直接进入 HA。
- Bluetooth Proxy：支持 BLE 广播转发和主动 GATT 连接；配置 3 个连接槽，ESP32 每槽约占 1 KB RAM。
- Apple HomeKit：通过 HAP-ESPHome 原生接入 Apple「家庭」，映射温度、湿度、CO₂、PM2.5。
- TVOC 和甲醛继续由 HA 提供，不会伪装成不匹配的 HomeKit 传感器类型。
- 移除 Web Server，为 BLE 与 HomeKit 留出内存。

## 在 Home Assistant ESPHome 中导入

本分支以 **ESPHome 2026.10.1** 为编译基线，三个配置都使用仓库内的 External Components，不再使用已移除的 `platform: custom` / `custom_component`。

1. 在 HA 的 ESPHome Dashboard 中选择导入配置（Import from URL）。
2. 根据设备型号使用对应 Raw YAML 地址：

   - BRY88AA151K：<https://raw.githubusercontent.com/doa2011/daikin-air-sensor-homekit-ble/feature/homekit-ble-proxy/daikin-bry88aa151k.yaml>
   - BRY88AB151K：<https://raw.githubusercontent.com/doa2011/daikin-air-sensor-homekit-ble/feature/homekit-ble-proxy/daikin-bry88ab151k.yaml>
   - BRY88AB151K-CH2O：<https://raw.githubusercontent.com/doa2011/daikin-air-sensor-homekit-ble/feature/homekit-ble-proxy/daikin-bry88ab151k-ch2o.yaml>
3. 在 ESPHome 配置目录中创建 `secrets.yaml`，填入 Wi-Fi 和 HomeKit 配对码：

```yaml
wifi_ssid: "你的 Wi-Fi 名称"
wifi_password: "你的 Wi-Fi 密码"
homekit_setup_code: "321-54-987"
```

HomeKit 配对码必须符合 `XXX-XX-XXX` 格式。请更换示例码；不要将真实 `secrets.yaml` 上传到 GitHub。

4. 保存后编译。首次刷写建议通过 USB 串口完成；后续可使用 OTA。

## 蓝牙连接配置

```yaml
esp32_ble:
  max_connections: 3

bluetooth_proxy:
  active: true
  connection_slots: 3
```

ESPHome 会通过 HA 蓝牙集成汇总多个 Proxy。GATT 连接槽只限制同时活动连接数；只广播、不建立 GATT 连接的传感器不占用连接槽。

## 首次 HomeKit 配对

设备连接 Wi-Fi 后，在 ESPHome 日志中查找 HomeKit 启动信息。打开 Apple「家庭」App 添加配件，输入 `secrets.yaml` 中的配对码。HomeKit 与 HA Native API 是两条独立接入路径，可以同时使用。

## 编译验证

GitHub Actions 对三个 YAML 分别执行 ESPHome 配置检查和固件编译。只有三个型号全部通过后，才将该分支标记为可部署版本。

## 上游项目

- 原始传感器项目：<https://github.com/louliangsheng/daikin-air-sensor>
- HAP-ESPHome：<https://github.com/rednblkx/HAP-ESPHome>
- ESPHome Bluetooth Proxy：<https://esphome.io/components/bluetooth_proxy/>
