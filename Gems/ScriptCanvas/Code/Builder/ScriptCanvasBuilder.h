/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <ScriptCanvas/Asset/RuntimeAsset.h>
#include <ScriptCanvas/Grammar/PrimitivesDeclarations.h>
#include <ScriptCanvas/Variable/VariableCore.h>

namespace ScriptCanvasEditor
{
    class EditorAssetTree;
}

namespace ScriptCanvasBuilder
{
    class BuildVariableOverrides
    {
    public:
        AZ_TYPE_INFO(BuildVariableOverrides, "{8336D44C-8EDC-4C28-AEB4-3420D5FD5AE2}");
        AZ_CLASS_ALLOCATOR(BuildVariableOverrides, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* reflectContext);

        void Clear();

        // use this to preserve old values that may have been overridden on the instance, and are still valid in the parsed graph
        void CopyPreviousOverriddenValues(const BuildVariableOverrides& source);

        bool IsEmpty() const;

        // use this to initialize the new data, and make sure they have a editor graph variable for proper editor display
        void PopulateFromParsedResults(ScriptCanvas::Grammar::AbstractCodeModelConstPtr abstractCodeModel, const ScriptCanvas::VariableData& variables);

        // #functions2 provide an identifier for the node/variable in the source that caused the dependency. the root will not have one.
        ScriptCanvasEditor::SourceHandle m_source;
        
        // all of the variables here are overrides
        AZStd::vector<ScriptCanvas::GraphVariable> m_variables;
        // the values here may or may not be overrides
        AZStd::vector<AZStd::pair<ScriptCanvas::VariableId, AZ::EntityId>> m_entityIds;
        // these two variable lists are all that gets exposed to the edit context
        AZStd::vector<ScriptCanvas::GraphVariable> m_overrides;
        AZStd::vector<ScriptCanvas::GraphVariable> m_overridesUnused;
        AZStd::vector<BuildVariableOverrides> m_dependencies;
    };

    // copy the variables overridden during editor / prefab build time back to runtime data
    ScriptCanvas::RuntimeDataOverrides ConvertToRuntime(const BuildVariableOverrides& overrides);

    AZ::Outcome<BuildVariableOverrides, AZStd::string> ParseEditorAssetTree(const ScriptCanvasEditor::EditorAssetTree& editorAssetTree);
}
