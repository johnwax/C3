#include "StdAfx.h"
#include "ConnectorBridge.h"
#include "GateRelay.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MWR::C3::Core::ConnectorBridge::ConnectorBridge(std::shared_ptr<GateRelay>&& gateway, std::shared_ptr<AbstractConnector>&& connector, std::string name, HashT nameHash)
	: m_GateRelay{ gateway }
	, m_Connector{ std::move(connector) }
	, m_Name{ name }
	, m_NameHash{ nameHash }
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MWR::C3::Core::ConnectorBridge::OnAttach()
{
	GetConnector()->OnAttach(shared_from_this());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MWR::C3::Core::ConnectorBridge::Detach()
{
	m_IsAlive = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MWR::C3::Core::ConnectorBridge::PostCommandToBinder(ByteView binderId, ByteView command)
{
	return GetGateRelay()->PostCommandToPeripheral(command, RouteId::FromByteView(binderId));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MWR::C3::Core::ConnectorBridge::OnCommandFromBinder(ByteView binderId, ByteView command)
{
	return GetConnector()->OnCommandFromBinder(binderId, command);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MWR::ByteVector MWR::C3::Core::ConnectorBridge::RunCommand(ByteView command)
{
	return GetConnector()->OnRunCommand(command);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MWR::C3::Core::ConnectorBridge::Log(LogMessage const& message)
{
	if (auto gate = GetGateRelay())
		gate->Log(message);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string MWR::C3::Core::ConnectorBridge::GetName() const
{
	return m_Name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MWR::HashT MWR::C3::Core::ConnectorBridge::GetNameHash() const
{
	return m_NameHash;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<MWR::C3::AbstractConnector> MWR::C3::Core::ConnectorBridge::GetConnector() const
{
	return m_Connector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<MWR::C3::Core::GateRelay> MWR::C3::Core::ConnectorBridge::GetGateRelay() const
{
	return m_GateRelay.lock();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MWR::C3::Core::ConnectorBridge::SetErrorStatus(std::string_view errorMessage)
{
	m_Error = errorMessage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string MWR::C3::Core::ConnectorBridge::GetErrorStatus()
{
	return m_Error;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MWR::ByteVector MWR::C3::Core::ConnectorBridge::PeripheralCreationCommand(ByteView connectionId, ByteView data, bool isX64)
{
	return m_Connector->PeripheralCreationCommand(connectionId, data, isX64);
}