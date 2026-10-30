<p align="left"><img alt="OSPTEK" src="../../images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 1.96″ AMOLED 显示屏 410×502（AM196Q410502LK）</h1>

<p align="center"><b>AMOLED 模组 · QSPI · CO5300AF-32</b></p>

<p align="center"><a href="./README_EN.md">English</a> | 简体中文 · <a href="../../README.md">规格族索引</a></p>

<p align="center">
  <img alt="Interface: QSPI" src="https://img.shields.io/badge/Interface-QSPI-27AE60?style=flat-square" />
  <img alt="Driver: CO5300AF-32" src="https://img.shields.io/badge/Driver-CO5300AF--32-E7352C?style=flat-square" />
  <img alt="Touch: CST820" src="https://img.shields.io/badge/Touch-CST820-F39C12?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 1.96 寸 410×502 AMOLED 模组宣传图" src="./images/PNG-4.png" width="640" /></p>

## 目录

- [规格参数](#规格参数)
- [相关资料](#相关资料)
- [仓库结构](#仓库结构)
- [购买链接](#购买链接)
- [技术支持](#技术支持)

---

## 规格参数

| 项目 | 规格 |
| ---- | ---- |
| 尺寸 | 1.96 英寸 |
| 类型 | AMOLED |
| 分辨率 | 410×502 |
| 外形尺寸 | 35.36 x 44.07 mm |
| 有效显示区域 | 31.37 x 38.40 mm |
| 接口 | QSPI |
| 驱动 IC | CO5300AF-32 |
| 触摸驱动 | CST820 |

> 完整外形尺寸、FPC 定义、供电与时序以产品规格书 / 驱动手册为准。

## 相关资料

| 分类 | 文件 |
| ---- | ---- |
| 规格文档 | [`CO_5300_Datasheet_V0_00_20230328_07edb82936.pdf`](./datasheets/CO_5300_Datasheet_V0_00_20230328_07edb82936.pdf) |
| 规格文档 | [`DS_CST_820_V1_2_e0543732ca.pdf`](./datasheets/DS_CST_820_V1_2_e0543732ca.pdf) |
| 规格文档 | [`AM196Q410502LK.pdf`](./datasheets/AM196Q410502LK.pdf) |
| CAD 图纸 | [`OK-23GF024-04.pdf`](./cad/OK-23GF024-04.pdf) |
| 转接板资料 | [`转接板.png`](./adapter/%E8%BD%AC%E6%8E%A5%E6%9D%BF.png) |
| 转接板资料 | [`ADP-AMOLED-196&213-QSPI-24P-V1.0.pdf`](./adapter/ADP-AMOLED-196%26213-QSPI-24P-V1.0.pdf) |

## 仓库结构

```text
display-amoled-1.96inch-410x502/ # 仓库根（导航见 ../../README.md，示例工程见根目录 examples/）
└── versions/
    └── AM196Q410502LK/        # 本料号完整资料
        ├── README.md
        ├── README_EN.md
        ├── osptek.yml
        ├── images/
        ├── datasheets/       # 产品规格书 + 驱动/触摸 IC 数据手册
        ├── cad/              # 外形/盖板 CAD
        └── adapter/          # 转接板资料 + 转接板图片
```

## 购买链接

见[仓库首页](../../README.md#购买链接)。

## 技术支持

见[仓库首页](../../README.md#技术支持)。

---

<p align="center"><sub>© 2026 OSPTEK 鱼鹰光电 · 本仓库资料采用 CC BY 4.0 许可</sub></p>
