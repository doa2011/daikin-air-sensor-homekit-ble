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
- 增加局域网 Web 管理页面（ESPHome Web Server v2、Digest 认证、内置前端资源）；不启用 Web OTA。
- Web 页面可查看传感器、调节 number 参数、操作公开 switch/button，并查看设备诊断信息。
- 增加运行时 Heap Free、Heap Min Free、最大连续块、Heap 碎片率、主循环耗时和复位原因监测。
- CI 对三个型号执行固件编译及 `esphome analyze-memory`，分别上传构建产物和内存分析报告。

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

## 局域网 Web 管理

设备加入 Wi-Fi 后，在浏览器访问 `http://设备IP/`。页面使用 ESPHome Web Server v2，内置前端资源，无需浏览器访问外部 CDN。

- 使用 Digest 用户名/密码认证，凭据配置在 `secrets.yaml` 的 `web_username` 和 `web_password`。
- 仅建议在可信局域网访问；不要在路由器上做公网端口映射。
- 不启用 Web OTA，固件更新继续通过受保护的 ESPHome OTA / Native API 工作流。
- 页面显示所有非 internal 的实体，包括传感器、阈值 number、LED 指示开关、重启和校准按钮。
- 诊断实体每 10 秒更新：Heap Free、Heap Min Free、最大连续 Heap Block、Heap Fragmentation、Loop Time、设备信息和复位原因。

## 内存验证

GitHub Actions 对三个型号执行 `esphome compile` 和 `esphome analyze-memory`。每个型号分别上传固件构建目录和内存分析报告。CI 的静态分析不等于设备运行时实测；刷机后应分别记录待机、Web 页面打开、BLE 三个 GATT 连接活跃时的 Heap Free / Min Free / Max Block。

## 编译验证

GitHub Actions 对三个 YAML 分别执行 ESPHome 配置检查、固件编译和内存分析。三个型号全部通过后，才将该分支标记为可部署版本。

## 上游项目

- 原始传感器项目：<https://github.com/louliangsheng/daikin-air-sensor>
- HAP-ESPHome：<https://github.com/rednblkx/HAP-ESPHome>
- ESPHome Bluetooth Proxy：<https://esphome.io/components/bluetooth_proxy/>
