## 📁 Axolotool Project Structure & Architecture

```text
├── .github/
|   ├── assets/
├── assets/
|   ├── tabler/
├── src/
|   └── editor/
|       ├── ANodeEnvironment/
|       ├── AWindow/
|       ├── Utility/
|       ├── CMakeLists.txt
|       └── main.cpp
├── .gitattributes
├── .gitignore
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
└── README.md
```

### 📑 Table of Contents

* 🧩 [ANodeEnvironment/](#-anodeenvironment)
* 💻 [AWindow/](#-awindow)
* ⚙️ [Utility/](#%EF%B8%8F-utility)

---

<br/>

### 🧩 ANodeEnvironment/

| Prefix | Meaning | Description |
|:------:|:--------|:------------|
| `RG`   | **Registry** | Core registration object, cahe containers, function registers, and visual registers for visible&#x2060;/&#x2060;hidden UI items. |
| `ND`   | **Node&nbsp;Data** | SQLite database, state storage, transaction herpers, and thread connection pool. |
| `ST`   | **Streaming** | Producer-consumer thread queues for async load/save operations. |
| `VW`   | **View** | Qt GraphicsView scene management, dynamic visual components, and canvas UX. |

```text
├── Registry/
|   ├── details/
|   |   ├── Context.h
|   |   └── RegistryTraits.h
|   ├── registry_base/
|   |   └── RGBaseRegistry.h
|   ├── visual_registry/
|   |   └── RGVisualRegistry.h
|   └── ARegistry.h
├── Storage/
|   ├── cell/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Helper.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDCell.h
|   ├── global_source/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDGlobalSource.h
|   ├── node/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDNode.h
|   ├── node_source/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDNodeSource.h
|   ├── pin/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDPin.h
|   ├── pin_source/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDPinSource.h
|   ├── widget/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDWidget.h
|   ├── widget_source/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDWidgetSource.h
|   ├── wire/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDWire.h
|   ├── wire_source/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   ├── Create.h
|   |   |   ├── Delete.h
|   |   |   ├── Init.h
|   |   |   ├── Read.h
|   |   |   └── Update.h
|   |   └── NDWireSource.h
|   ├── ANodeEnvDB.h
|   ├── NDConcepts.h
|   ├── NDConfig.h
|   ├── NDHelpers.h
|   └── NDPool.h
├── Streaming/
|   ├── details/
|   |   ├── BoundedQueue.h
|   |   └── Helper.h
|   ├── loading_node_consumer/
|   |   └── STLoadingNodeConsumer.h
|   ├── loading_node_streamer/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   └── NodeProcessing.h
|   |   └── STLoadingNodeStreamer.h
|   ├── loading_wire_consumer/
|   |   └── STLoadingWireConsumer.h
|   ├── loading_wire_streamer/
|   |   ├── details/
|   |   |   ├── Config.h
|   |   |   └── WireProcessing.h
|   |   └── STLoadingWireStreamer.h
|   ├── saving_consumer/
|   |   ├── details/
|   |   |   └── SavingApplier.h
|   |   └── STSavingConsumer.h
|   ├── saving_streamer/
|   |   └── STSavingStreamer.h
|   └── STStreamingManager.h
├── View/
|   ├── canvas/
|   |   ├── details/
|   |   |   ├── CanvasView.h
|   |   └── VWCanvas.h
|   ├── cell/
|   |   ├── details/
|   |   |   ├── CellItem.h
|   |   |   ├── Concepts.h
|   |   |   ├── Context.h
|   |   |   ├── CreateCell.h
|   |   |   ├── CreatePin.h
|   |   |   ├── CreateWidget.h
|   |   |   └── Helpers.h
|   |   └── VWCell.h
|   ├── drag_drop/
|   |   ├── details/
|   |   |   └── Context.h
|   |   └── VWDragDrop.h
|   ├── node/
|   |   ├── details/
|   |   |   ├── Context.h
|   |   |   ├── CreateNode.h
|   |   |   ├── NodeGrid.h
|   |   |   └── NodeItem.h
|   |   └── VWNode.h
|   ├── pin/
|   |   ├── details/
|   |   |   ├── Concepts.h
|   |   |   ├── Context.h
|   |   |   ├── PinAllowSet.h
|   |   |   ├── PinData.h
|   |   |   └── PinItem.h
|   |   └── VWPin.h
|   ├── scene/
|   |   └── VWNodeScene.h
|   ├── widget/
|   |   ├── details/
|   |   |   ├── Context.h
|   |   |   └── WidgetWrapper.h
|   |   └── VWWidget.h
|   ├── wire/
|   |   ├── details/
|   |   |   ├── Concepts.h
|   |   |   ├── Context.h
|   |   |   ├── CreateWire.h
|   |   |   ├── Helpers.h
|   |   |   ├── WireItem.h
|   |   |   └── WireTemp.h
|   |   └── VWWire.h
|   └── AView.h
└── ANodeEnvironment.h
```

### 💻 AWindow/

```text
AWindow/
├── AMainWindow.h
├── AWindow.h
└── MockPlugin.h
```

### ⚙️ Utility/

```text
├── details/
|   ├── ByteArray.h
|   ├── SqlTransaction.h
|   ├── StylesUtility.h
|   └── UUID.h
└── Utility.h
```
