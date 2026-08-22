#pragma once

#include "../ANodeEnvironment/ANodeEnvironment.h"

namespace MockPlugin {

    namespace Color {
        inline QColor Execution = QColor::fromString("#ffffffff"); // Pure White
        inline QColor Class =     QColor::fromString("#ff00a6f2"); // Sky Blue
        inline QColor Object =    QColor::fromString("#ff00a6f2"); // Sky Blue

        inline QColor Bool =  QColor::fromString("#ff910000"); // Crimson
        inline QColor Byte =  QColor::fromString("#ff006d64"); // Forest Teal
        inline QColor Int =   QColor::fromString("#ff22e0ac"); // Mint Green
        inline QColor Int64 = QColor::fromString("#ffaae1ad"); // Pastel Mint

        inline QColor Float =  QColor::fromString("#ff9dfc43"); // Marigold
        inline QColor Vector = QColor::fromString("#fffbc627"); // Lime Green

        inline QColor Name =   QColor::fromString("#ffca80fb"); // Lavender
        inline QColor String = QColor::fromString("#fffd00d1"); // Magenta
    }

	inline bool initPlugin(ANodeEnvironment::ANodeEnvironment* nodeEnv) {
		using namespace ANodeEnvironment::Storage::Config;

		if (!nodeEnv) return false;

		auto* nodeEnvDB = nodeEnv->db();
		if (!nodeEnvDB) return false;

		auto& nodeRegistry = nodeEnv->registry();

		const bool sucess = ANodeEnvironment::Storage::Helpers::useTransaction(nodeEnvDB->getPool(), [&](QSqlQuery& query) -> bool {

			auto globalSourceID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-5367f7f33e62");
			if (!globalSourceID) return false;
			nodeEnvDB->globalSource.createGlobalSource(GlobalSource::CreateGlobalSourceRecord{
				.id = *globalSourceID,
				.name = "Standard",
				.dsc = "standard global source"
				});

            // Node Source ------------------------------------------------------------------------------------------------------------------------------------------
			auto createNodeSourceID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-5432b8b0a8b1");
			if (!createNodeSourceID) return false;
			nodeEnvDB->nodeSource.createNodeSource(query, NodeSource::CreateNodeSourceRecord{
				.id = *createNodeSourceID,
				.globalSourceId = *globalSourceID,
				.name = "Standard::Source[0]"
				});

			auto createNodeContributorID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-5b83e08903c2");
			if (!createNodeContributorID) return false;
			nodeEnvDB->nodeSource.createNodeContributor(query, NodeSource::CreateNodeContributorRecord{
				.id = *createNodeContributorID,
				.sourceId = *createNodeSourceID,
				.name = "Standard::Contributor[0]"
				});

			auto createNodeTypeID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-5e29b0c58b76");
			if (!createNodeTypeID) return false;
			nodeEnvDB->nodeSource.createNodeType(query, NodeSource::CreateNodeTypeRecord{
				.id = *createNodeTypeID,
				.contributorId = *createNodeContributorID,
				//.metadata = ,
				.name = "Standard::Type[0]"
				});

			auto createNodeDataID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-618b39cdf9eb");
			if (!createNodeDataID) return false;
			nodeEnvDB->nodeSource.createNodeData(query, NodeSource::CreateNodeDataRecord{
				.id = *createNodeDataID,
				.contributorId = *createNodeContributorID,
				//.data = ,
				.name = "Standard::Data[0]"
				});

            // Pin Source ------------------------------------------------------------------------------------------------------------------------------------------
			auto createPinSourceID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-67a4818eed9d");
			if (!createPinSourceID) return false;
			nodeEnvDB->pinSource.createPinSource(query, PinSource::CreatePinSourceRecord{
				.id = *createPinSourceID,
				.globalSourceId = *globalSourceID,
				.name = "Standard::Source[0]"
				});

			auto createPinContributorID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-68a2889ac0db");
			if (!createPinContributorID) return false;
			nodeEnvDB->pinSource.createContributor(query, PinSource::CreatePinContributorRecord{
				.id = *createPinContributorID,
				.sourceId = *createPinSourceID,
				.name = "Standard::Contributor[0]"
				});

            // Pin Source Flow ------------------------------------------------------------------------------------------------------------------------------------------
			auto createPinOutFlowID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-6d477e5dbd7d");
			if (!createPinOutFlowID) return false;
			nodeEnvDB->pinSource.createFlow(query, PinSource::CreatePinFlowRecord{
				.id = *createPinOutFlowID,
				.contributorId = *createPinContributorID,
				.name = "Standard::Out",
				.degree = 0.0,
				});
			auto createPinInFlowID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-73302c1d52e7");
			if (!createPinInFlowID) return false;
			nodeEnvDB->pinSource.createFlow(query, PinSource::CreatePinFlowRecord{
				.id = *createPinInFlowID,
				.contributorId = *createPinContributorID,
				.name = "Standard::In",
				.degree = 180.0,
				});

            // Pin Source Type ------------------------------------------------------------------------------------------------------------------------------------------
            auto createPinExecutionTypeID = muuid::uuid::from_chars("019f65ff-1fe9-7f9d-a47e-1d6cceff0eb0");
            if (!createPinExecutionTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinExecutionTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Execution",
                .bit_size = 0
                });
            auto createPinClassTypeID = muuid::uuid::from_chars("01a011a9-3ab2-725d-a8df-ef6dc182b916");
            if (!createPinClassTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinClassTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Class",
                .bit_size = 0
                });
            auto createPinObjectTypeID = muuid::uuid::from_chars("019f65ff-1fe9-77a7-ba63-dbfd2f8b7a62");
            if (!createPinObjectTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinObjectTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Object",
                .bit_size = 0
                });
            auto createPinBoolTypeID = muuid::uuid::from_chars("019f65ff-1fe9-7391-a581-22392c3e6bb8");
            if (!createPinBoolTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinBoolTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Bool",
                .bit_size = 1
                });
            auto createPinByteTypeID = muuid::uuid::from_chars("019f65ff-1fe9-78f3-a827-202e61a9028e");
            if (!createPinByteTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinByteTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Byte",
                .bit_size = 8
                });
            auto createPinIntTypeID = muuid::uuid::from_chars("019f65ff-1fe9-782a-84e4-b3e0aa66aaaf");
            if (!createPinIntTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinIntTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Int",
                .bit_size = 32
                });
            auto createPinInt64TypeID = muuid::uuid::from_chars("019f65ff-1fe9-75ce-8513-2f9c49ccfdd0");
            if (!createPinInt64TypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinInt64TypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Int64",
                .bit_size = 64
                });
            auto createPinFloatTypeID = muuid::uuid::from_chars("019f65ff-1fe9-7fcf-ada1-7c80bb07dc44");
            if (!createPinFloatTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinFloatTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Float",
                .bit_size = 32
                });
            auto createPinDoubleTypeID = muuid::uuid::from_chars("019f65ff-1fe9-77e4-bf10-7c3bf96f8d75");
            if (!createPinDoubleTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinDoubleTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Double",
                .bit_size = 64
                });
            auto createPinVector2TypeID = muuid::uuid::from_chars("019f65ff-1fe9-726b-9d16-d0996c448a5a");
            if (!createPinVector2TypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinVector2TypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Vector2",
                .bit_size = 128
                });
            auto createPinVector3TypeID = muuid::uuid::from_chars("019f65ff-1fe9-70a1-aa07-bc78a953737d");
            if (!createPinVector3TypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinVector3TypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Vector3",
                .bit_size = 192
                });
            auto createPinVectorTypeID = muuid::uuid::from_chars("019f65ff-1fe9-7c8d-8603-c504a6eb210d");
            if (!createPinVectorTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinVectorTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Vector",
                .bit_size = -1
                });
            auto createPinNameTypeID = muuid::uuid::from_chars("019f65ff-1fe9-750a-9772-d732c2d1dba4");
            if (!createPinNameTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinNameTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Name",
                .bit_size = -1
                });
            auto createPinStringTypeID = muuid::uuid::from_chars("019f65ff-1fe9-7b94-ba7c-2adf429f1b53");
            if (!createPinStringTypeID) return false;
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = *createPinStringTypeID,
                .contributorId = *createPinContributorID,
                .name = "Standard::String",
                .bit_size = -1
                });

            // Pin Source Type (End) ------------------------------------------------------------------------------------------------------------------------------------------

            // Pin Source style ------------------------------------------------------------------------------------------------------------------------------------------
            auto createPinExecutionStyleID = muuid::uuid::from_chars("019f65ff-1fe9-72ce-b064-d16e00ca0b4f");
            if (!createPinExecutionStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinExecutionStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Execution",
                .color = MockPlugin::Color::Execution,
                .wireThickness = 2,
                });
            auto createPinClassStyleID = muuid::uuid::from_chars("019f65ff-1fe9-73e9-83d3-d235e9aa8ba3");
            if (!createPinClassStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinClassStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Class",
                .color = MockPlugin::Color::Class,
                .wireThickness = 2,
                });
            auto createPinObjectStyleID = muuid::uuid::from_chars("019f65ff-1fe9-72d0-b4c4-1e01db3ec35b");
            if (!createPinObjectStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinObjectStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Object",
                .color = MockPlugin::Color::Object,
                .wireThickness = 2,
                });
            auto createPinBoolStyleID = muuid::uuid::from_chars("019f65ff-1fe9-7ca5-a824-5021a987e625");
            if (!createPinBoolStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinBoolStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Bool",
                .color = MockPlugin::Color::Bool,
                .wireThickness = 2,
                });
            auto createPinByteStyleID = muuid::uuid::from_chars("019f65ff-1fe9-7a49-a6bc-c0c0f21091d5");
            if (!createPinByteStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinByteStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Byte",
                .color = MockPlugin::Color::Byte,
                .wireThickness = 2,
                });
            auto createPinIntStyleID = muuid::uuid::from_chars("019f65ff-1fe9-7df4-9095-76f514383811");
            if (!createPinIntStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinIntStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Int",
                .color = MockPlugin::Color::Int,
                .wireThickness = 2,
                });
            auto createPinInt64StyleID = muuid::uuid::from_chars("019f65ff-1fe9-70f3-8742-f51366d1ea06");
            if (!createPinInt64StyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinInt64StyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Int64",
                .color = MockPlugin::Color::Int64,
                .wireThickness = 2,
                });
            auto createPinFloatStyleID = muuid::uuid::from_chars("019f65ff-1fe9-7ee2-bada-895eacc8d346");
            if (!createPinFloatStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinFloatStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Float",
                .color = MockPlugin::Color::Float,
                .wireThickness = 2,
                });
            auto createPinVectorStyleID = muuid::uuid::from_chars("019f65ff-1fe9-7e06-b429-e6c87b23e1fd");
            if (!createPinVectorStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinVectorStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Vector",
                .color = MockPlugin::Color::Vector,
                .wireThickness = 2,
                });
            auto createPinNameStyleID = muuid::uuid::from_chars("019f65ff-1fe9-70e6-944e-876ed74d148a");
            if (!createPinNameStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinNameStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::Name",
                .color = MockPlugin::Color::Name,
                .wireThickness = 2,
                });
            auto createPinStringStyleID = muuid::uuid::from_chars("019f65ff-1fe9-748b-a0d0-267b6f0dc66f");
            if (!createPinStringStyleID) return false;
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = *createPinStringStyleID,
                .contributorId = *createPinContributorID,
                .name = "Standard::String",
                .color = MockPlugin::Color::String,
                .wireThickness = 2,
                });

            // Pin Source Style (End) ------------------------------------------------------------------------------------------------------------------------------------------

            // Widget Source ------------------------------------------------------------------------------------------------------------------------------------------
            auto createWidgetSourceID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-76c066425d99");
            if (!createWidgetSourceID) return false;
            nodeEnvDB->widgetSource.createWidgetSource(query, WidgetSource::CreateWidgetSourceRecord{
                .id = *createWidgetSourceID,
                .globalSourceId = *globalSourceID,
                .name = "Standard::Source[0]"
                });

            auto createWidgetContributorID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-78854d262d95");
            if (!createWidgetContributorID) return false;
            nodeEnvDB->widgetSource.createWidgetContributor(query, WidgetSource::CreateWidgetContributorRecord{
                .id = *createWidgetContributorID,
                .sourceId = *createWidgetSourceID,
                .name = "Standard::Source[0]"
                });

            auto createWidgetTypeID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-7c673c9e4344");
            if (!createWidgetTypeID) return false;
            nodeEnvDB->widgetSource.createWidgetType(query, WidgetSource::CreateWidgetTypeRecord{
                .id = *createWidgetTypeID,
                .contributorId = *createWidgetContributorID,
                .name = "Standard::Type[0]"
                //.metadata = 
                });

            auto createWidgetDataID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-810fe3244564");
            if (!createWidgetDataID) return false;
            nodeEnvDB->widgetSource.createWidgetData(query, WidgetSource::CreateWidgetDataRecord{
                .id = *createWidgetDataID,
                .contributorId = *createWidgetContributorID,
                .name = "Standard::Data[0]"
                //.data =
                });


            // Wire Source ------------------------------------------------------------------------------------------------------------------------------------------
            auto createWireSourceID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-84932e8cd569");
            if (!createWireSourceID) return false;
            nodeEnvDB->wireSource.createWireSource(query, WireSource::CreateWireSourceRecord{
                .id = *createWireSourceID,
                .globalSourceId = *globalSourceID,
                .name = "Standard::Source[0]"
                });

            auto createWireContributorID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-8964fd612b87");
            if (!createWireContributorID) return false;
            nodeEnvDB->wireSource.createWireContributor(query, WireSource::CreateWireContributorRecord{
                .id = *createWireContributorID,
                .sourceId = *createWireSourceID,
                .name = "Standard::Contrubutor[0]"
                });

            // Wire Source Data ------------------------------------------------------------------------------------------------------------------------------------------
            auto createWireDataID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-8c53b4516c09");
            if (!createWireDataID) return false;
            nodeEnvDB->wireSource.createWireData(query, WireSource::CreateWireDataRecord{
                .id = *createWireDataID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Data[0]"
                //.data = 
                });

            // Wire Source Style ------------------------------------------------------------------------------------------------------------------------------------------
            auto createWireExecutionStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-911157333e59");
            if (!createWireExecutionStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireExecutionStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Execution",
                .color = MockPlugin::Color::Execution,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireClassStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-96c4e9938398");
            if (!createWireClassStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireClassStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Class",
                .color = MockPlugin::Color::Class,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireObjectStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-99f9fc1f99ed");
            if (!createWireObjectStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireObjectStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Object",
                .color = MockPlugin::Color::Object,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireBoolStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-9cd221f7a17d");
            if (!createWireBoolStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireBoolStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Bool",
                .color = MockPlugin::Color::Bool,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireByteStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-a3743b00ee5e");
            if (!createWireByteStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireByteStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Byte",
                .color = MockPlugin::Color::Byte,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireIntStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-a432a121407f");
            if (!createWireIntStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireIntStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Int",
                .color = MockPlugin::Color::Int,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireInt64StyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-abcec1393781");
            if (!createWireInt64StyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireInt64StyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Int64",
                .color = MockPlugin::Color::Int64,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireFloatStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-ace7a8820a7f");
            if (!createWireFloatStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireFloatStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Float",
                .color = MockPlugin::Color::Float,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireVectorStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-b233a73acbe0");
            if (!createWireVectorStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireVectorStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Vector",
                .color = MockPlugin::Color::Vector,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireNameStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-b793238be096");
            if (!createWireNameStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireNameStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::Name",
                .color = MockPlugin::Color::Name,
                .wireThickness = 2
                //.metadata = 
                });
            auto createWireStringStyleID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-ba13c9f67211");
            if (!createWireStringStyleID) return false;
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = *createWireStringStyleID,
                .contributorId = *createWireContributorID,
                .name = "Standard::String",
                .color = MockPlugin::Color::String,
                .wireThickness = 2
                //.metadata = 
                });

            // Wire Source Style (End) ------------------------------------------------------------------------------------------------------------------------------------------

            // Wire Visual Factory ------------------------------------------------------------------------------------------------------------------------------------------

            const auto createWireConfiguratorID = muuid::uuid::from_chars("01a01708-add2-75ac-b870-db4c3ee26920");
            if (!createWireConfiguratorID) return false;
            std::function<bool(QGraphicsItem* wire, VWWireDetails::Context::WireContext wireContext)> wireConfigurator = [&](QGraphicsItem* wire, VWWireDetails::Context::WireContext wireContext) {

                //wireContext.name;

                if (wireContext.style) {
                    auto wireItem = dynamic_cast<QGraphicsPathItem*>(wire);
                    if (!wireItem) return false;

                    QPen pen;
                    pen.setColor(wireContext.style->color);
                    pen.setWidthF(wireContext.style->wireThickness);

                    pen.setCapStyle(Qt::RoundCap);
                    pen.setJoinStyle(Qt::RoundJoin);

                    wireItem->setPen(pen);
                }
                if (wireContext.data) {
                    //wireContext.data->data
                }

                return true;
                };
            if (!nodeRegistry.wireFunction.wireConfiguratorRegistry.insert(*createWireConfiguratorID, wireConfigurator)) return false;

            // Wire Core ------------------------------------------------------------------------------------------------------------------------------------------
            auto createExecutionWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-bc9a3626fc05");
            if (!createExecutionWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createExecutionWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Execution",
                .styleId = *createWireExecutionStyleID
                });
            auto createClassWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-c06dced4d2f0");
            if (!createClassWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createClassWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Class",
                .styleId = *createWireClassStyleID
                //.metadata = 
                });
            auto createObjectWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-c53e1df31757");
            if (!createObjectWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createObjectWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Object",
                .styleId = *createWireObjectStyleID
                //.metadata = 
                });
            auto createBoolWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-c9181ab6857b");
            if (!createBoolWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createBoolWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Bool",
                .styleId = *createWireBoolStyleID
                //.metadata = 
                });
            auto createByteWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-ce5b7d51d367");
            if (!createByteWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createByteWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Byte",
                .styleId = *createWireByteStyleID
                //.metadata = 
                });
            auto createIntWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-d058df6eb2ac");
            if (!createIntWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createIntWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Int",
                .styleId = *createWireIntStyleID
                //.metadata = 
                });
            auto createInt64WireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-d6b8d63e5176");
            if (!createInt64WireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createInt64WireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Int64",
                .styleId = *createWireInt64StyleID
                //.metadata = 
                });
            auto createFloatWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-da02a04469a5");
            if (!createFloatWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createFloatWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Float",
                .styleId = *createWireFloatStyleID
                //.metadata = 
                });
            auto createVectorWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-dc9d6bab10a5");
            if (!createVectorWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createVectorWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Vector",
                .styleId = *createWireVectorStyleID
                //.metadata = 
                });
            auto createNameWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-e3af40c63f39");
            if (!createNameWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createNameWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::Name",
                .styleId = *createWireNameStyleID
                //.metadata = 
                });
            auto createStringWireCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-e624a4ddf755");
            if (!createStringWireCoreID) return false;
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = *createStringWireCoreID,
                .contributorId = *createWireContributorID,
                .visualFactoryId = *createWireConfiguratorID,
                .name = "Standard::String",
                .styleId = *createWireStringStyleID
                //.metadata = 
                });

            // Wire Core (End) ------------------------------------------------------------------------------------------------------------------------------------------

            // Widget Core ------------------------------------------------------------------------------------------------------------------------------------------
            auto createWidgetCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-ea878bbc5632");
            if (!createWidgetCoreID) return false;
            nodeEnvDB->widget.createWidgetCore(query, Widget::CreateWidgetCoreRecord{
                .id = *createWidgetCoreID,
                .contributorId = *createWidgetContributorID,
                //.typeId = ,
                //.dataId = ,
                //.visualFactoryId = 
                });


            // pinConfiguratorRegistry ------------------------------------------------------------------------------------------------------------------------------------------
            auto pinConfiguratorID = muuid::uuid::from_chars("01a01038-f084-7167-a126-1523448df0b3");
            if (!pinConfiguratorID) return false;

            auto pinConfiguratorFn = [&](QGraphicsRectItem* nodeCell, QGraphicsItem* pin, VWPinDetails::Context::PinContext pinContext, const std::optional<QString> text) -> bool {

                const auto m_createPinInFlowID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-73302c1d52e7");
                const auto m_createPinOutFlowID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-6d477e5dbd7d");

                if (!nodeCell || !pin || !m_createPinInFlowID || !m_createPinOutFlowID) return false;

                const QRectF rawPinRect = pin->boundingRect();
                const QRectF visualPinRect = rawPinRect.adjusted(8.0, 8.0, -8.0, -8.0);

                QGraphicsTextItem* label = new QGraphicsTextItem(nodeCell);
                label->setDefaultTextColor(Qt::white);
                label->setFlag(QGraphicsItem::ItemIsMovable, false);
                label->setFlag(QGraphicsItem::ItemIsSelectable, false);
                label->document()->setDocumentMargin(0);

                if (text) label->setPlainText(*text);

                constexpr qreal cellHeight = 20.0;
                constexpr qreal spacing = 6.0; 

                // ---------------- IN-FLOW PIN ----------------
                if (pinContext.pinflow && pinContext.pinflow->id == *m_createPinInFlowID) {
                    qreal pinX = 0.0 - visualPinRect.left();
                    qreal pinY = (cellHeight / 2.0) - rawPinRect.center().y();
                    pin->setPos(pinX, pinY);

                    qreal pinVisualRight = pinX + visualPinRect.right();
                    qreal labelX = pinVisualRight + spacing;
                    qreal labelY = (cellHeight / 2.0) - label->boundingRect().center().y();
                    label->setPos(labelX, labelY);

                    qreal totalWidth = labelX + label->boundingRect().width() + spacing;
                    nodeCell->setRect(0, 0, totalWidth, cellHeight);

                    return true;
                }

                // ---------------- OUT-FLOW PIN ----------------
                if (pinContext.pinflow && pinContext.pinflow->id == *m_createPinOutFlowID) {
                    qreal labelX = spacing;
                    qreal labelY = (cellHeight / 2.0) - label->boundingRect().center().y();
                    label->setPos(labelX, labelY);

                    qreal labelRight = labelX + label->boundingRect().width();
                    qreal pinX = (labelRight + spacing) - visualPinRect.left();
                    qreal pinY = (cellHeight / 2.0) - rawPinRect.center().y();
                    pin->setPos(pinX, pinY);

                    qreal totalWidth = pinX + visualPinRect.right();
                    nodeCell->setRect(0, 0, totalWidth, cellHeight);

                    return true;
                }

                delete label;
                return false;
                };
            nodeRegistry.nodeFunction.pinConfiguratorRegistry.insert(*pinConfiguratorID, pinConfiguratorFn);

            // Pin Core ------------------------------------------------------------------------------------------------------------------------------------------
            auto createPinExecutionOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-ee4bb1cdb136");
            if (!createPinExecutionOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinExecutionOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinExecutionTypeID,
                .styleId = *createPinExecutionStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinExecutionTypeID},
                });
            auto createPinExecutionInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-f0377f85fd63");
            if (!createPinExecutionInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinExecutionInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinExecutionTypeID,
                .styleId = *createPinExecutionStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinExecutionTypeID}
                });

            auto createPinClassOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-f662f3449865");
            if (!createPinClassOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinClassOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinClassTypeID,
                .styleId = *createPinClassStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinClassTypeID}
                });
            auto createPinClassInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-fbf20d9084cf");
            if (!createPinClassInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinClassInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinClassTypeID,
                .styleId = *createPinClassStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinClassTypeID}
                });

            auto createPinObjectOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8213-fc09c3f92cab");
            if (!createPinObjectOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinObjectOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinObjectTypeID,
                .styleId = *createPinObjectStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinObjectTypeID}
                });
            auto createPinObjectInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-0370100f4e4f");
            if (!createPinObjectInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinObjectInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinObjectTypeID,
                .styleId = *createPinObjectStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinObjectTypeID}
                });

            auto createPinBoolOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-0619b71d11da");
            if (!createPinBoolOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinBoolOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinBoolTypeID,
                .styleId = *createPinBoolStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinBoolTypeID}
                });
            auto createPinBoolInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-0b6289444f45");
            if (!createPinBoolInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinBoolInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinBoolTypeID,
                .styleId = *createPinBoolStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinBoolTypeID}
                });

            auto createPinByteOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-0fa291014139");
            if (!createPinByteOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinByteOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinByteTypeID,
                .styleId = *createPinByteStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinByteTypeID}
                });
            auto createPinByteInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-12ab09ab2805");
            if (!createPinByteInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinByteInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinByteTypeID,
                .styleId = *createPinByteStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinByteTypeID}
                });

            auto createPinIntOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-1434f616f62f");
            if (!createPinIntOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinIntOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinIntTypeID,
                .styleId = *createPinIntStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinIntTypeID}
                });
            auto createPinIntInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-19435996d0ed");
            if (!createPinIntInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinIntInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinIntTypeID,
                .styleId = *createPinIntStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinIntTypeID}
                });

            auto createPinInt64OutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-1dc21752ca55");
            if (!createPinInt64OutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinInt64OutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinInt64TypeID,
                .styleId = *createPinInt64StyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinInt64TypeID}
                });
            auto createPinInt64InCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-20c2e7119492");
            if (!createPinInt64InCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinInt64InCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinInt64TypeID,
                .styleId = *createPinInt64StyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinInt64TypeID}
                });

            auto createPinFloatOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-2659a7affa11");
            if (!createPinFloatOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinFloatOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinFloatTypeID,
                .styleId = *createPinFloatStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinFloatTypeID}
                });
            auto createPinFloatInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-2a88da1418f6");
            if (!createPinFloatInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinFloatInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinFloatTypeID,
                .styleId = *createPinFloatStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinFloatTypeID}
                });

            auto createPinDoubleOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-2c97ef05f751");
            if (!createPinDoubleOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinDoubleOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinDoubleTypeID,
                .styleId = *createPinFloatStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinDoubleTypeID}
                });
            auto createPinDoubleInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-3169910f3b67");
            if (!createPinDoubleInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinDoubleInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinDoubleTypeID,
                .styleId = *createPinFloatStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinDoubleTypeID}
                });

            auto createPinVectorOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-36f4ce7439a0");
            if (!createPinVectorOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinVectorOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinVectorTypeID,
                .styleId = *createPinVectorStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinVectorTypeID}
                });
            auto createPinVectorInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-38bd2199a746");
            if (!createPinVectorInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinVectorInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinVectorTypeID,
                .styleId = *createPinVectorStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinVectorTypeID}
                });
            auto createPinVector2OutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-3f86bcf85023");
            if (!createPinVector2OutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinVector2OutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinVector2TypeID,
                .styleId = *createPinVectorStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinVector2TypeID}
                });
            auto createPinVector2InCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-43c5a6be4e9a");
            if (!createPinVector2InCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinVector2InCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinVector2TypeID,
                .styleId = *createPinVectorStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinVector2TypeID}
                });
            auto createPinVector3OutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-4746a4fe7e07");
            if (!createPinVector3OutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinVector3OutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinVector3TypeID,
                .styleId = *createPinVectorStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinVector3TypeID}
                });
            auto createPinVector3InCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-4ab57c2a21d4");
            if (!createPinVector3InCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinVector3InCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinVector3TypeID,
                .styleId = *createPinVectorStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinVector3TypeID}
                });

            auto createPinNameOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-4f5a92062224");
            if (!createPinNameOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinNameOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinNameTypeID,
                .styleId = *createPinNameStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinNameTypeID}
                });
            auto createPinNameInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-5266d5e6454e");
            if (!createPinNameInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinNameInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinNameTypeID,
                .styleId = *createPinNameStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinNameTypeID}
                });

            auto createPinStringOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-57d76b387d8c");
            if (!createPinStringOutCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinStringOutCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinOutFlowID,
                .typeId = *createPinStringTypeID,
                .styleId = *createPinStringStyleID,
                .allowFlowList = QList{*createPinInFlowID},
                .allowTypeList = QList{*createPinStringTypeID}
                });
            auto createPinStringInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-5b94c538d9e6");
            if (!createPinStringInCoreID) return false;
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = *createPinStringInCoreID,
                .contributorId = *createPinContributorID,
                .visualFactoryId = *pinConfiguratorID,
                .flowId = *createPinInFlowID,
                .typeId = *createPinStringTypeID,
                .styleId = *createPinStringStyleID,
                .allowFlowList = QList{*createPinOutFlowID},
                .allowTypeList = QList{*createPinStringTypeID}
                });

            // Pin Core (End) ------------------------------------------------------------------------------------------------------------------------------------------


            // Node Core ------------------------------------------------------------------------------------------------------------------------------------------
            const auto createNodeACoreId = muuid::uuid::from_chars("01a01024-8932-7727-9b20-30a7f81ad76a");
            const auto createNodeBCoreId = muuid::uuid::from_chars("01a01029-f8e6-73e9-9db3-926c8ad12f52");
            const auto createNodeCCoreId = muuid::uuid::from_chars("01a01029-f8e6-73e9-9db3-964174d5c085");

            if (!createNodeACoreId || !createNodeBCoreId || !createNodeCCoreId) return false;

            nodeEnvDB->node.createNodeCore(query, Node::CreateNodeCoreRecord{
                .id = *createNodeACoreId,
                .contributorId = *createNodeContributorID,
                .typeId = *createNodeTypeID,
                .dataId = *createNodeDataID,
                .name = "Node A",
                .defaultRowNum = 1,
                .defaultColNum = 1,
                .defaultNodeWidth = -1.0,
                .defaultNodeHeight = -1.0,
                .cellVisualFallbackId = std::nullopt,
                });
            nodeEnvDB->node.createNodeCore(query, Node::CreateNodeCoreRecord{
                .id = *createNodeBCoreId,
                .contributorId = *createNodeContributorID,
                .typeId = *createNodeTypeID,
                .dataId = *createNodeDataID,
                .name = "Node B",
                .defaultRowNum = 1,
                .defaultColNum = 2,
                .defaultNodeWidth = -1.0,
                .defaultNodeHeight = -1.0,
                .cellVisualFallbackId = std::nullopt,
                });
            nodeEnvDB->node.createNodeCore(query, Node::CreateNodeCoreRecord{
                .id = *createNodeCCoreId,
                .contributorId = *createNodeContributorID,
                .typeId = *createNodeTypeID,
                .dataId = *createNodeDataID,
                .name = "Node C",
                .defaultRowNum = 1,
                .defaultColNum = 1,
                .defaultNodeWidth = -1.0,
                .defaultNodeHeight = -1.0,
                .cellVisualFallbackId = std::nullopt,
                });

            // Node Function Registry ------------------------------------------------------------------------------------------------------------------------------------------
            bool cellFactoryRegistrySuccess = true;

            // --- Node A Factory ---
            std::function<std::vector<ARegistryDetails::Context::CreateCellRecord>(const muuid::uuid& nodeId)> NodeAFn = [&](const muuid::uuid& nodeId) {
                std::vector<ARegistryDetails::Context::CreateCellRecord> cells;

                auto m_createPinIntOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-1434f616f62f");
                if (!m_createPinIntOutCoreID) return cells;

                cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                    .id = muuid::uuid::generate_unix_time_based(),
                    .nodeId = nodeId,
                    .name = "Node A Out",
                    .pinCoreId = *m_createPinIntOutCoreID,
                    .widgetCoreId = std::nullopt,
                    .row = 0,
                    .col = 0,
                    .rowSpan = 1,
                    .colSpan = 1
                    });
                return cells;
                };
            if (!nodeRegistry.nodeFunction.cellFactoryRegistry.insert(*createNodeACoreId, NodeAFn)) cellFactoryRegistrySuccess = false;

            // --- Node B Factory ---
            std::function<std::vector<ARegistryDetails::Context::CreateCellRecord>(const muuid::uuid& nodeId)> NodeBFn = [&](const muuid::uuid& nodeId) {
                std::vector<ARegistryDetails::Context::CreateCellRecord> cells;

                auto m_createPinIntInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-19435996d0ed");
                auto m_createPinIntOutCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-1434f616f62f");
                if (!m_createPinIntInCoreID || !m_createPinIntOutCoreID) return cells;

                cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                    .id = muuid::uuid::generate_unix_time_based(),
                    .nodeId = nodeId,
                    .name = "Node B In",
                    .pinCoreId = *m_createPinIntInCoreID,
                    .widgetCoreId = std::nullopt,
                    .row = 0,
                    .col = 0,
                    .rowSpan = 1,
                    .colSpan = 1
                    });
                cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                    .id = muuid::uuid::generate_unix_time_based(),
                    .nodeId = nodeId,
                    .name = "Node B Out",
                    .pinCoreId = *m_createPinIntOutCoreID,
                    .widgetCoreId = std::nullopt,
                    .row = 0,
                    .col = 1,
                    .rowSpan = 1,
                    .colSpan = 1
                    });

                return cells;
                };
            if (cellFactoryRegistrySuccess && !nodeRegistry.nodeFunction.cellFactoryRegistry.insert(*createNodeBCoreId, NodeBFn)) cellFactoryRegistrySuccess = false;

            // --- Node C Factory ---
            std::function<std::vector<ARegistryDetails::Context::CreateCellRecord>(const muuid::uuid& nodeId)> NodeCFn = [&](const muuid::uuid& nodeId) {
                std::vector<ARegistryDetails::Context::CreateCellRecord> cells;

                auto m_createPinIntInCoreID = muuid::uuid::from_chars("01a0100a-e34c-76f8-8214-19435996d0ed");
                if (!m_createPinIntInCoreID) return cells;

                cells.emplace_back(ARegistryDetails::Context::CreateCellRecord{
                    .id = muuid::uuid::generate_unix_time_based(),
                    .nodeId = nodeId,
                    .name = "Node C In",
                    .pinCoreId = *m_createPinIntInCoreID,
                    .widgetCoreId = std::nullopt,
                    .row = 0,
                    .col = 0,
                    .rowSpan = 1,
                    .colSpan = 1
                    });
                return cells;
                };
            if (cellFactoryRegistrySuccess && !nodeRegistry.nodeFunction.cellFactoryRegistry.insert(*createNodeCCoreId, NodeCFn)) cellFactoryRegistrySuccess = false;

            if (!cellFactoryRegistrySuccess) {
                if (createNodeACoreId) nodeRegistry.nodeFunction.cellFactoryRegistry.erase(*createNodeACoreId);
                if (createNodeBCoreId) nodeRegistry.nodeFunction.cellFactoryRegistry.erase(*createNodeBCoreId);
                if (createNodeCCoreId) nodeRegistry.nodeFunction.cellFactoryRegistry.erase(*createNodeCCoreId);

                return false;
            }

            // Node Function Registry (End) ------------------------------------------------------------------------------------------------------------------------------------------


            //nodeRegistry.nodeFunction.cellFallbackConfiguratorRegistry;
            
            //nodeRegistry.nodeFunction.widgetFactoryRegistry;


            // Wire Registry ------------------------------------------------------------------------------------------------------------------------------------------

            bool symmetricWireMapSuccess = true;

            if (!nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinExecutionTypeID, *createExecutionWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinClassTypeID,     *createClassWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinObjectTypeID,    *createObjectWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinBoolTypeID,      *createBoolWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinByteTypeID,      *createByteWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinIntTypeID,       *createIntWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinInt64TypeID,     *createInt64WireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinFloatTypeID,     *createFloatWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinDoubleTypeID,    *createFloatWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinVectorTypeID,    *createVectorWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinVector2TypeID,   *createVectorWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinVector3TypeID,   *createVectorWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinNameTypeID,      *createNameWireCoreID)  ||
                !nodeRegistry.wireFunction.symmetricWireMap.insert(*createPinStringTypeID,    *createStringWireCoreID)
            ) symmetricWireMapSuccess = false;

            if (!symmetricWireMapSuccess) {
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinExecutionTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinClassTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinObjectTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinBoolTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinByteTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinIntTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinInt64TypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinFloatTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinDoubleTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinVectorTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinVector2TypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinVector3TypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinNameTypeID);
                nodeRegistry.wireFunction.symmetricWireMap.erase(*createPinStringTypeID);

                return false;
            }

            /*
            nodeRegistry.wireFunction.asymmetricWireMap;
            */
            return true;
        });
        return sucess;
    }
}