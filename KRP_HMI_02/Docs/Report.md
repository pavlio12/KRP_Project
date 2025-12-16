# Dual‑Role USB Host/Device with TouchGFX HMI  
**Course:** Komunikační rozhraní počítačů (CTU FEE)  
**Author:** Ondřej Pavlín, KyR (Cybernetics & Robotics), CTU FEE, Prague  
**Platform:** STM32H747I‑DISCO (CM7) with TouchGFX  
**Date:** 2025

---

## 1. Purpose and Scope
This report documents the design, integration, and stabilization of a dual‑role USB solution (Device + Host) with a TouchGFX user interface on the STM32H747I‑DISCO. It highlights architectural choices, toolchain constraints, and corrective actions taken to achieve reliable enumeration and data display for a university‑level project on USB protocols.

## 2. Development Process and Toolchain
- **Base project:** Generated with TouchGFX Designer (UI scaffolding: Screen1/Screen2).  
- **CubeMX/STM32CubeIDE:** Initially used to enable USB **Device** via `.ioc` and generate code.  
- **Manual Host integration:** USB **Host** stack was copied in (Core, Class, Target/App) and hand‑wired. This required:
  - Adding Host include/source paths to MCU GCC/G++ compilers.
  - Adding linked resources so STM32CubeIDE sees imported folders.
  - Reconciling HAL/LL init, interrupts, and BSP glue.
- **Important constraint:** Re‑generating code from `.ioc` now breaks the project (hundreds of errors) because CubeMX overwrites manual Host additions and include paths. TouchGFX Designer remains usable. Any CubeMX changes must be generated in a throwaway project and ported selectively.

## 3. High‑Level Architecture
### Application/UI (TouchGFX)
- **Model/Presenter/View:**  
  - Screen1: USB role/state + device info (TextArea).  
  - Screen2: System messages + USB state graph.  
  - `hmiBridge` C wrappers expose model setters to firmware (role/state, system messages, device info).

### USB Subsystem
- **Device role:** CDC/ACM (buffers aligned for HS DMA).  
- **Host role:** MSC/HID classes with added diagnostics and stall detection.
- **DRD control:** `usb_task.c` selects role and runs host/device tasks.

### Middleware/HAL
- **USB Host Library (ST):**  
  - Control/config/data buffers aligned (32‑byte) for DMA/cache safety.  
  - Unaligned access fixes (byte‑wise parsing/copy for packed descriptors and CBW/CSW).  
  - Deferred string‑descriptor reads outside user callback (avoid re‑entrant control).  
  - Enumeration watchdog logging.  
  - Cold‑start sequence: OTG HS + ULPI RCC reset, VBUS off/on with delays, enforced port reset.
- **USB Device Library (ST):**  
  - CDC/HID with aligned endpoint buffers.
- **BSP:** OTG HS (ULPI) initialized with explicit reset and VBUS sequencing; IRQ handlers mapped; optional VBUS event queue.

## 4. Key Design Decisions and Rationale
- **Hybrid project (TouchGFX + manual Host glue):** Enables DRD without regenerating `.ioc`, at the cost of forbidding automatic codegen.  
- **Alignment discipline:** All buffers touched by OTG HS DMA are 32‑byte aligned; packed structures are written byte‑wise. UNALIGN_TRP remains enabled to surface bugs.  
- **Control‑path safety:** String descriptor reads deferred until control engine idle; avoids deadlocks/hardfaults.  
- **Cold‑start robustness:** NRST alone left OTG/ULPI and devices in undefined states; explicit peripheral reset + VBUS power‑cycle + port reset makes NRST behave like power‑cycle.  
- **Logging:** Host state, enum state, stall details routed to HMI for field diagnosis.

## 5. Issues Encountered and Solutions
| Issue | Root Cause | Fix |
| --- | --- | --- |
| HardFaults during MSC enumeration | Unaligned word writes into packed CBW/Inquiry structures with UNALIGN_TRP set | Align BOT buffers; byte‑wise clear/copy for CBW and inquiry fields |
| Enumeration stalls after warm reset | OTG HS/ULPI and device not truly reset on NRST; device remained in addressed state | Force RCC reset of OTG HS + ULPI; VBUS off/on; mandatory port reset with long delays |
| .ioc regeneration breaks build | CubeMX overwrites manual Host integration and include paths | Treat `.ioc` as read‑only; use throwaway projects to diff and copy specific changes |
| UI device info missing | Model gating updates when Screen1 inactive; some calls to empty listener | Always dispatch `setDeviceInfo` to current listener; store last info and replay on Screen1 activation |

## 6. Validation
- **Host enumeration:** Tested with HID, MSC flash drive. After cold‑start sequence, enumeration is reliable (no stalls/hardfaults).  
- **Warm reset:** Previously stalled; now succeeds due to reset/power‑cycle logic.  
- **HMI:** Screen1 shows role/state/device info; Screen2 logs system messages and USB state graph.

## 7. Limitations and Risks
- `.ioc` generator cannot be rerun on this project without manual reconciliation.  
- `USBH_LL_DriverVBUS` still lacks hardware VBUS switch control (GPIO/ULPI EVBUS) in BSP; port may rely on board default wiring.  
- Additional classes/buffers must follow alignment/byte‑wise write rules; regressions will reintroduce faults.  
- If ULPI reset/VBUS timing is shortened, re‑verify warm‑reset behavior.

## 8. Future Work
- Implement real VBUS drive in `USBH_LL_DriverVBUS` (GPIO or ULPI EVBUS) and expose a GUI indicator (HPRT or driver state).  
- Document a “no‑regen” warning in the repository root; script to diff CubeMX throwaway outputs vs. curated tree.  
- Add lightweight self‑tests for alignment (static assertions, buffer alignment checks) and control‑path sanity.  
- Optimize cold‑start delays once long‑delay configuration is proven stable.

## 9. Conclusions
The project demonstrates a stable dual‑role USB implementation with a responsive TouchGFX HMI on STM32H747I‑DISCO. Success depended on strict buffer alignment, byte‑wise handling of packed USB structures, deferring control‑path operations, and enforcing a true cold‑start sequence for the OTG HS/ULPI subsystem. The hybrid nature of the codebase (TouchGFX + hand‑integrated Host) requires disciplined maintenance: avoid `.ioc` regeneration, preserve alignment practices, and manage VBUS/reset sequencing explicitly.
