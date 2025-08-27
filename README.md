进test里 看config.py的提示
需要python3.9版本建议使用anaconda
可以搜一下教程
创建之后可以pip安装时使用shiyan.py的命令直接复制粘贴
模型是yoloV8
启动的时候只需要运行 python MainProgram.py


## 项目简介

本项目提供一个基于 YOLOv8 的 PyQt5 可视化检测程序，并通过串口与 STM32F103C8T6 下位机进行通信。支持图片、视频、摄像头实时检测，可保存检测结果，并将满足条件的目标类别通过串口发送给 MCU。

PC 端主要代码位于 `test/` 目录；下位机 Keil 工程位于 `串口/` 目录。

## 目录结构（摘录）

- `test/`
	- `MainProgram.py`：PyQt5 主程序入口
	- `Config.py`：模型与类别配置、保存目录配置
	- `detect_tools.py`：图像绘制与工具函数
	- `best.pt`：默认检测模型（YOLOv8 权重）
	- `yolov8n.pt`：示例 YOLOv8n 权重
	- `Font/platech.ttf`：中文字体文件
- `串口/`：STM32F103C8T6 Keil 工程（用于接收串口数据）

## 环境要求

- 操作系统：Windows 10/11
- Python：3.9（推荐 Anaconda 管理环境）
- 显卡：可选（若安装 GPU 版 PyTorch 可获得更快推理）

## 安装步骤

1) 创建并激活 Python 3.9 环境（推荐 Anaconda）：

```powershell
conda create -n yolo-pyqt python=3.9 -y
conda activate yolo-pyqt
```

2) 安装依赖（ultralytics 会同时安装 torch/torchvision 的合适版本；若失败，按提示单独安装对应平台的 PyTorch）：

```powershell
pip install ultralytics==8.* opencv-python PyQt5 Pillow numpy
```

3) 首次运行前，请在 `test/` 同级目录创建检测结果保存文件夹（或在 `Config.py` 中修改保存路径）：

```powershell
mkdir .\test\save_data
```

## 运行方式（PC 端）

请从仓库根目录进入 `test/` 后运行主程序：

```powershell
cd .\test
python .\MainProgram.py
```

程序启动后即可通过 GUI：

- 打开图片：按钮 `PicBtn`
- 打开视频：按钮 `VideoBtn`
- 打开/关闭摄像头：按钮 `CapBtn`
- 批量检测文件夹图片：按钮 `FilesBtn`
- 保存检测结果（图片/视频）：按钮 `SaveBtn`
- 退出：按钮 `ExitBtn`

检测结果会同时显示：

- 目标框与标签（中文），
- 当前帧耗时，
- 检测数量，
- 进度条显示各类别占比，
- 下方表格记录路径、类别、置信度与坐标。

## 配置说明（`test/Config.py`）

- `save_path`：保存图片/视频检测结果的输出目录（默认 `save_data`）。
- `model_path`：YOLOv8 权重路径（默认 `best.pt`，相对 `test/` 目录）。
- `names`：类别索引到英文名称的映射。
- `CH_names`：类别索引到中文名称的映射（与 `names` 顺序保持一致）。

如需更换模型或类别：

1) 将新权重文件放入 `test/`（或填写绝对路径），并把 `model_path` 指向新权重；
2) 按新数据集类别顺序同步修改 `names` 与 `CH_names`。

## 串口通信与协议

GUI 内置串口面板：

- 端口选择：下拉框 `comboBox11`（自动枚举可用串口）。
- 打开/关闭串口：按钮 `pushButton`（文本在“打开串口/关闭串口”之间切换）。
- 波特率、校验位、数据位、停止位：在右侧参数下拉框中选择（由 UI 提供）。

发送策略（见 `MainProgram.py`）：

- 每帧检测后，过滤掉“类别索引 == 0”的目标；
- 置信度 ≥ 0.7 的目标会被加入发送列表；
- 最终以“逗号分隔的类别索引字符串+换行”发送，例如：`1,2,5\n`；
- 发送成功与否会在串口日志框内回显。

请在下位机端按行读取，并以逗号分割成整型数组进行解析。

## 类别定义（默认）

索引 → 英文/中文：

- 0: Trafic Light Signal / 交通信号灯
- 1: Stop Signal / 停止信号
- 2: Speedlimit Signal / 限速信号
- 3: Crosswalk Signal / 人行横道信号
- 4: Crosswalk / 人行横道
- 5: Pedestrian / 行人
- 6: Bus / 公交车
- 7: Car / 汽车
- 8: Truck / 卡车

注意：串口发送时会忽略索引 0 的目标（仅用于 GUI 显示）。

## 保存检测结果

- 图片：在单张或批量检测后点击 `SaveBtn`，会将绘制了目标框的图片保存到 `save_path`。文件名在原名后附加 `_detect_result`。
- 视频：保存前会提示耗时，确认后对整段视频逐帧推理并写入 `XVID` 编码的 `*.avi` 文件，保存在 `save_path`。
- 摄像头流：不支持直接保存（按钮会给出提示）。

若保存失败，请检查 `save_path` 是否存在，或是否有写入权限。

## STM32F103C8T6 端（`串口/`）

- 使用 Keil uVision 打开 `串口/Project.uvprojx` 工程。
- 串口波特率、校验位、数据位、停止位需与 PC 端保持一致。
- 在接收回调中按行读取数据，基于 `,` 与 `\n` 进行分割解析为整型列表。
- 结合 `Config.py` 的类别定义进行业务处理；如更改了 PC 端类别，请同步更新下位机解析/显示逻辑（`main.c` 等处）。

## 常见问题（FAQ）

- 运行时报错“找不到 QtSerialPort/模块”：请确保安装的是 `PyQt5>=5.15`；重新安装 `PyQt5` 后重试。
- Ultralytics/torch 安装失败：按官方提示为当前 Python/CUDA 版本单独安装对应的 `torch/torchvision` 后，再安装 `ultralytics`。
- 权重未找到：请确认 `test/best.pt` 是否存在，或修改 `Config.model_path` 指向实际路径。
- 字体缺失导致中文不显示：确认 `test/Font/platech.ttf` 存在且路径未改动。
- 保存失败：创建 `test/save_data` 目录，或修改 `Config.save_path` 到可写目录。

## 快速上手步骤回顾

1) 准备 Python 3.9 环境并安装依赖；
2) 确认 `test/` 下存在 `best.pt` 与 `Font/platech.ttf`；
3) 创建 `test/save_data`；
4) 在 `test/` 下运行 `python MainProgram.py`；
5) 在 GUI 中选择串口并打开，开始检测并联调 MCU。

—— 如果你只想快速试跑：直接进入 `test/`，运行 `python MainProgram.py` 即可。
