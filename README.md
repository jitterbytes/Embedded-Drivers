# Embedded Drivers

Building portable embedded drivers from scratch, focusing on clean architecture, reusability, and cross-platform support.

## 📌 Overview
This repository contains platform-independent drivers for sensors and peripherals.  
Each driver is designed with a clear separation between core logic and platform-specific implementation.

## 🎯 Goals
- Develop reusable and portable embedded drivers
- Maintain clean hardware abstraction (HAL-based design)
- Support multiple platforms (Arduino, ESP-IDF, etc.)
- Build a strong foundation in embedded systems design

## 📂 Structure
Each driver follows a consistent structure:

- `driver/` → Core driver logic (platform-independent)  
- `platform/` → Platform-specific implementations  
- `examples/` → Usage examples for different platforms  

## 🚧 Status
This project is currently in early development. Drivers and platform support will be added progressively.

## 📜 License
MIT License