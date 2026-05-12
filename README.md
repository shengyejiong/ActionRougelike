# ActionRougelike

一个基于 Unreal Engine 5 C++ 的第三人称动作 Roguelike 学习项目。

本项目主要作为个人练习与课程跟做记录，核心内容围绕角色动作系统、AI 敌人、投射物、交互、强化道具和基础存档功能展开。

## Project Overview

- Engine: Unreal Engine 5.0.3
- Language: C++
- Project Type: Third-person action roguelike prototype
- Purpose: Learning Unreal Engine gameplay programming and project structure

## Implemented Features

- 第三人称角色移动与视角控制
- 普通攻击、冲刺、投射物攻击
- 自定义 `ActionComponent` 动作系统
- AI 敌人、行为树与 EQS 刷怪逻辑
- 可交互物体与强化道具
- 基础属性系统
- 基础存档与加载逻辑
- 部分网络复制相关实现

## Course Reference

本项目参考并跟做了课程：

`Professional Game Development in C++ and Unreal Engine`

这里的代码与工程结构主要用于个人学习、练习和复盘 Unreal Engine C++ 开发流程。

## Known Issues

当前已知问题如下：

1. rage 条不更新
2. 在引擎内启动游戏并执行相关操作后会报错：`未指定生成类`
3. 火堆保存点触发后未常驻燃烧

## Running the Project

1. 使用 Unreal Engine 5.0.3 打开 `ActionRougelike.uproject`
2. 等待工程生成并编译完成
3. 从编辑器内启动 Play 进行测试

## Repository Notes

- 这是一个学习用途项目，不是商业成品
- 仓库中可能保留了学习阶段的实验性实现和未修复问题
- 项目命名中的 `Rougelike` 保留了当前工程原名

## Disclaimer

This repository is a personal learning project created while following a game development course. It is shared for study and portfolio purposes only and is not affiliated with the course publisher.
