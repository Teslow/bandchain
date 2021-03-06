module RawDataReport = {
  type t = {
    externalDataID: int,
    exitCode: int,
    data: JsBuffer.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      externalDataID: json |> intWithDefault(field("external_id")),
      exitCode: json |> intWithDefault(field("exit_code")),
      data: json |> bufferWithDefault(field("data")),
    };
};

module Msg = {
  type badge_t =
    | SendBadge
    | ReceiveBadge
    | CreateDataSourceBadge
    | EditDataSourceBadge
    | CreateOracleScriptBadge
    | EditOracleScriptBadge
    | RequestBadge
    | ReportBadge
    | AddReporterBadge
    | RemoveReporterBadge
    | CreateValidatorBadge
    | EditValidatorBadge
    | CreateClientBadge
    | UpdateClientBadge
    | SubmitClientMisbehaviourBadge
    | ConnectionOpenInitBadge
    | ConnectionOpenTryBadge
    | ConnectionOpenAckBadge
    | ConnectionOpenConfirmBadge
    | ChannelOpenInitBadge
    | ChannelOpenTryBadge
    | ChannelOpenAckBadge
    | ChannelOpenConfirmBadge
    | ChannelCloseInitBadge
    | ChannelCloseConfirmBadge
    | PacketBadge
    | AcknowledgementBadge
    | TimeoutBadge
    | DelegateBadge
    | UndelegateBadge
    | RedelegateBadge
    | WithdrawRewardBadge
    | UnjailBadge
    | SetWithdrawAddressBadge
    | SubmitProposalBadge
    | DepositBadge
    | VoteBadge
    | WithdrawCommissionBadge
    | MultiSendBadge
    | ActivateBadge
    | UnknownBadge;

  type msg_cat_t =
    | TokenMsg
    | ValidatorMsg
    | ProposalMsg
    | DataMsg
    | UnknownMsg;

  let getBadgeVariantFromString = badge => {
    switch (badge) {
    | "send" => SendBadge
    | "receive" => raise(Not_found)
    | "create_data_source" => CreateDataSourceBadge
    | "edit_data_source" => EditDataSourceBadge
    | "create_oracle_script" => CreateOracleScriptBadge
    | "edit_oracle_script" => EditOracleScriptBadge
    | "request" => RequestBadge
    | "report" => ReportBadge
    | "add_reporter" => AddReporterBadge
    | "remove_reporter" => RemoveReporterBadge
    | "create_validator" => CreateValidatorBadge
    | "edit_validator" => EditValidatorBadge
    | "create_client" => CreateClientBadge
    | "update_client" => UpdateClientBadge
    | "submit_client_misbehaviour" => SubmitClientMisbehaviourBadge
    | "connection_open_init" => ConnectionOpenInitBadge
    | "connection_open_try" => ConnectionOpenTryBadge
    | "connection_open_ack" => ConnectionOpenAckBadge
    | "connection_open_confirm" => ConnectionOpenConfirmBadge
    | "channel_open_init" => ChannelOpenInitBadge
    | "channel_open_try" => ChannelOpenTryBadge
    | "channel_open_ack" => ChannelOpenAckBadge
    | "channel_open_confirm" => ChannelOpenConfirmBadge
    | "channel_close_init" => ChannelCloseInitBadge
    | "channel_close_confirm" => ChannelCloseConfirmBadge
    | "ics04/opaque" => PacketBadge
    | "ics04/timeout" => TimeoutBadge
    | "acknowledgement" => AcknowledgementBadge
    | "delegate" => DelegateBadge
    | "begin_unbonding" => UndelegateBadge
    | "begin_redelegate" => RedelegateBadge
    | "withdraw_delegator_reward" => WithdrawRewardBadge
    | "unjail" => UnjailBadge
    | "set_withdraw_address" => SetWithdrawAddressBadge
    | "submit_proposal" => SubmitProposalBadge
    | "deposit" => DepositBadge
    | "vote" => VoteBadge
    | "withdraw_validator_commission" => WithdrawCommissionBadge
    | "multisend" => MultiSendBadge
    | "activate" => ActivateBadge
    | _ => UnknownBadge
    };
  };

  module Send = {
    type t = {
      fromAddress: Address.t,
      toAddress: Address.t,
      amount: list(Coin.t),
    };

    let decode = json =>
      JsonUtils.Decode.{
        fromAddress: json |> at(["msg", "from_address"], string) |> Address.fromBech32,
        toAddress: json |> at(["msg", "to_address"], string) |> Address.fromBech32,
        amount: json |> at(["msg", "amount"], list(Coin.decodeCoin)),
      };
  };

  module Receive = {
    type t = {
      fromAddress: Address.t,
      toAddress: Address.t,
      amount: list(Coin.t),
    };
  };

  module CreateDataSource = {
    type success_t = {
      id: ID.DataSource.t,
      owner: Address.t,
      name: string,
      executable: JsBuffer.t,
      sender: Address.t,
    };

    type fail_t = {
      owner: Address.t,
      name: string,
      executable: JsBuffer.t,
      sender: Address.t,
    };

    let decodeSuccess = json =>
      JsonUtils.Decode.{
        id: json |> at(["extra", "id"], ID.DataSource.fromJson),
        owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
        name: json |> at(["msg", "name"], string),
        executable: json |> at(["msg", "executable"], string) |> JsBuffer.fromBase64,
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };

    let decodeFail = json =>
      JsonUtils.Decode.{
        owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
        name: json |> at(["msg", "name"], string),
        executable: json |> at(["msg", "executable"], string) |> JsBuffer.fromBase64,
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };
  };

  module EditDataSource = {
    type t = {
      id: ID.DataSource.t,
      owner: Address.t,
      name: string,
      executable: JsBuffer.t,
      sender: Address.t,
    };

    let decode = json =>
      JsonUtils.Decode.{
        id: json |> at(["msg", "data_source_id"], ID.DataSource.fromJson),
        owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
        name: json |> at(["msg", "name"], string),
        executable: json |> at(["msg", "executable"], string) |> JsBuffer.fromBase64,
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };
  };

  module CreateOracleScript = {
    type success_t = {
      id: ID.OracleScript.t,
      owner: Address.t,
      name: string,
      code: JsBuffer.t,
      sender: Address.t,
    };

    type fail_t = {
      owner: Address.t,
      name: string,
      code: JsBuffer.t,
      sender: Address.t,
    };

    let decodeSuccess = json =>
      JsonUtils.Decode.{
        id: json |> at(["extra", "id"], ID.OracleScript.fromJson),
        owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
        name: json |> at(["msg", "name"], string),
        code: json |> at(["msg", "code"], string) |> JsBuffer.fromBase64,
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };

    let decodeFail = json =>
      JsonUtils.Decode.{
        owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
        name: json |> at(["msg", "name"], string),
        code: json |> at(["msg", "code"], string) |> JsBuffer.fromBase64,
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };
  };

  module EditOracleScript = {
    type t = {
      id: ID.OracleScript.t,
      owner: Address.t,
      name: string,
      code: JsBuffer.t,
      sender: Address.t,
    };

    let decode = json =>
      JsonUtils.Decode.{
        id: json |> at(["msg", "oracle_script_id"], ID.OracleScript.fromJson),
        owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
        name: json |> at(["msg", "name"], string),
        code: json |> at(["msg", "code"], string) |> JsBuffer.fromBase64,
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };
  };

  module Request = {
    type success_t = {
      id: ID.Request.t,
      oracleScriptID: ID.OracleScript.t,
      oracleScriptName: string,
      calldata: JsBuffer.t,
      askCount: int,
      minCount: int,
      schema: string,
      sender: Address.t,
    };

    type fail_t = {
      oracleScriptID: ID.OracleScript.t,
      calldata: JsBuffer.t,
      askCount: int,
      minCount: int,
      sender: Address.t,
    };

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        id: json |> at(["extra", "id"], ID.Request.fromJson),
        oracleScriptID: json |> at(["msg", "oracle_script_id"], ID.OracleScript.fromJson),
        oracleScriptName: json |> at(["extra", "name"], string),
        calldata: json |> bufferWithDefault(at(["msg", "calldata"])),
        askCount: json |> at(["msg", "ask_count"], int),
        minCount: json |> at(["msg", "min_count"], int),
        schema: json |> at(["extra", "schema"], string),
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        oracleScriptID: json |> at(["msg", "oracle_script_id"], ID.OracleScript.fromJson),
        calldata: json |> bufferWithDefault(at(["msg", "calldata"])),
        askCount: json |> at(["msg", "ask_count"], int),
        minCount: json |> at(["msg", "min_count"], int),
        sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      };
    };
  };

  module Report = {
    type t = {
      requestID: ID.Request.t,
      rawReports: list(RawDataReport.t),
      validator: Address.t,
      reporter: Address.t,
    };

    let decode = json =>
      JsonUtils.Decode.{
        requestID: json |> at(["msg", "request_id"], ID.Request.fromJson),
        rawReports: json |> at(["msg", "raw_reports"], list(RawDataReport.decode)),
        validator: json |> at(["msg", "validator"], string) |> Address.fromBech32,
        reporter: json |> at(["msg", "reporter"], string) |> Address.fromBech32,
      };
  };
  module AddReporter = {
    type success_t = {
      validator: Address.t,
      reporter: Address.t,
      validatorMoniker: string,
    };

    type fail_t = {
      validator: Address.t,
      reporter: Address.t,
    };

    let decodeSuccess = json =>
      JsonUtils.Decode.{
        validator: json |> at(["msg", "validator"], string) |> Address.fromBech32,
        reporter: json |> at(["msg", "reporter"], string) |> Address.fromBech32,
        validatorMoniker: json |> at(["extra", "validator_moniker"], string),
      };

    let decodeFail = json =>
      JsonUtils.Decode.{
        validator: json |> at(["msg", "validator"], string) |> Address.fromBech32,
        reporter: json |> at(["msg", "reporter"], string) |> Address.fromBech32,
      };
  };

  module RemoveReporter = {
    type success_t = {
      validator: Address.t,
      reporter: Address.t,
      validatorMoniker: string,
    };

    type fail_t = {
      validator: Address.t,
      reporter: Address.t,
    };

    let decodeSuccess = json =>
      JsonUtils.Decode.{
        validator: json |> at(["msg", "validator"], string) |> Address.fromBech32,
        reporter: json |> at(["msg", "reporter"], string) |> Address.fromBech32,
        validatorMoniker: json |> at(["extra", "validator_moniker"], string),
      };

    let decodeFail = json =>
      JsonUtils.Decode.{
        validator: json |> at(["msg", "validator"], string) |> Address.fromBech32,
        reporter: json |> at(["msg", "reporter"], string) |> Address.fromBech32,
      };
  };

  module CreateValidator = {
    type t = {
      moniker: string,
      identity: string,
      website: string,
      details: string,
      commissionRate: float,
      commissionMaxRate: float,
      commissionMaxChange: float,
      delegatorAddress: Address.t,
      validatorAddress: Address.t,
      publicKey: PubKey.t,
      minSelfDelegation: Coin.t,
      selfDelegation: Coin.t,
    };
    let decode = json =>
      JsonUtils.Decode.{
        moniker: json |> at(["msg", "description", "moniker"], string),
        identity: json |> at(["msg", "description", "identity"], string),
        website: json |> at(["msg", "description", "website"], string),
        details: json |> at(["msg", "description", "details"], string),
        commissionRate: json |> at(["msg", "commission", "rate"], floatstr),
        commissionMaxRate: json |> at(["msg", "commission", "max_rate"], floatstr),
        commissionMaxChange: json |> at(["msg", "commission", "max_change_rate"], floatstr),
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        publicKey: json |> at(["msg", "pubkey"], string) |> PubKey.fromBech32,
        minSelfDelegation:
          json |> at(["msg", "min_self_delegation"], floatstr) |> Coin.newUBANDFromAmount,
        selfDelegation: json |> at(["msg", "value"], Coin.decodeCoin),
      };
  };

  module EditValidator = {
    type t = {
      moniker: string,
      identity: string,
      website: string,
      details: string,
      commissionRate: option(float),
      sender: Address.t,
      minSelfDelegation: option(Coin.t),
    };

    let decode = json => {
      exception WrongNetwork(string);
      switch (Env.network) {
      | "GUANYU"
      | "GUANYU38" =>
        JsonUtils.Decode.{
          moniker: json |> at(["msg", "description", "moniker"], string),
          identity: json |> at(["msg", "description", "identity"], string),
          website: json |> at(["msg", "description", "website"], string),
          details: json |> at(["msg", "description", "details"], string),
          commissionRate: json |> optional(at(["msg", "commission_rate"], floatstr)),
          sender: json |> at(["msg", "address"], string) |> Address.fromBech32,
          minSelfDelegation:
            json
            |> optional(at(["msg", "min_self_delegation"], floatstr))
            |> Belt.Option.map(_, Coin.newUBANDFromAmount),
        }
      | "WENCHANG" =>
        JsonUtils.Decode.{
          moniker: json |> at(["msg", "moniker"], string),
          identity: json |> at(["msg", "identity"], string),
          website: json |> at(["msg", "website"], string),
          details: json |> at(["msg", "details"], string),
          commissionRate: json |> optional(at(["msg", "commission_rate"], floatstr)),
          sender: json |> at(["msg", "address"], string) |> Address.fromBech32,
          minSelfDelegation:
            json
            |> optional(at(["msg", "min_self_delegation"], floatstr))
            |> Belt.Option.map(_, Coin.newUBANDFromAmount),
        }
      | _ => raise(WrongNetwork("Incorrect or unspecified NETWORK environment variable"))
      };
    };
  };

  module CreateClient = {
    type t = {
      address: Address.t,
      clientID: string,
      chainID: string,
      trustingPeriod: MomentRe.Duration.t,
      unbondingPeriod: MomentRe.Duration.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        address: json |> field("address", string) |> Address.fromBech32,
        clientID: json |> field("client_id", string),
        chainID: "band-consumer",
        trustingPeriod:
          (json |> field("trusting_period", JsonUtils.Decode.float))
          /. 1_000_000.
          |> MomentRe.durationMillis,
        unbondingPeriod:
          (json |> field("unbonding_period", JsonUtils.Decode.float))
          /. 1_000_000.
          |> MomentRe.durationMillis,
      };
    };
  };

  module UpdateClient = {
    type t = {
      address: Address.t,
      clientID: string,
      chainID: string,
      validatorHash: Hash.t,
      prevValidatorHash: Hash.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        address: json |> field("address", string) |> Address.fromBech32,
        clientID: json |> field("client_id", string),
        chainID: "band-consumer",
        validatorHash:
          "88A40098ACD2B9DDBD81E1397217BD0BC6D5B90C86D3614927B2CCDF4A3023BD" |> Hash.fromBase64,
        prevValidatorHash:
          "88A40098ACD2B9DDBD81E1397217BD0BC6D5B90C86D3614927B2CCDF4A3023BD" |> Hash.fromBase64,
      };
    };
  };

  module SubmitClientMisbehaviour = {
    type t = {
      address: Address.t,
      clientID: string,
      chainID: string,
      validatorHash: Hash.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        address: json |> field("address", string) |> Address.fromBech32,
        clientID: json |> field("client_id", string),
        chainID: "band-consumer",
        validatorHash:
          "88A40098ACD2B9DDBD81E1397217BD0BC6D5B90C86D3614927B2CCDF4A3023BD" |> Hash.fromBase64,
      };
    };
  };

  module Packet = {
    type common_t = {
      sequence: int,
      sourcePort: string,
      sourceChannel: string,
      destinationPort: string,
      destinationChannel: string,
      timeoutHeight: int,
      chainID: string,
    };

    type t = {
      sender: Address.t,
      data: string,
      common: common_t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        sender: json |> field("signer", string) |> Address.fromBech32,
        data: json |> at(["packet", "data"], string) |> Js.String.toUpperCase,
        common: {
          sequence: json |> at(["packet", "sequence"], int),
          sourcePort: json |> at(["packet", "source_port"], string),
          sourceChannel: json |> at(["packet", "source_channel"], string),
          destinationPort: json |> at(["packet", "destination_port"], string),
          destinationChannel: json |> at(["packet", "destination_channel"], string),
          timeoutHeight: json |> at(["packet", "timeout_height"], int),
          chainID: "band-consumer",
        },
      };
    };
  };

  module Acknowledgement = {
    type t = {
      common: Packet.common_t,
      sender: Address.t,
      acknowledgement: string,
    };
    let decode = json =>
      JsonUtils.Decode.{
        common: {
          sequence: 999,
          sourcePort: "gjdojfpjfp",
          sourceChannel: "gjdojfpjfp",
          destinationPort: "gjdojfpjfp",
          destinationChannel: "gjdojfpjfp",
          timeoutHeight: 999999,
          chainID: "band-consumer",
        },
        sender: json |> field("address", string) |> Address.fromBech32,
        acknowledgement: "iKQAmKzSud29geE5che9C8bVuQyG02FJJ7LM...",
      };
  };

  module Timeout = {
    type t = {
      sender: Address.t,
      common: Packet.common_t,
      nextSequenceReceive: int,
    };
    let decode = json =>
      JsonUtils.Decode.{
        sender: json |> field("signer", string) |> Address.fromBech32,
        common: {
          sequence: json |> at(["packet", "sequence"], int),
          sourcePort: json |> at(["packet", "source_port"], string),
          sourceChannel: json |> at(["packet", "source_channel"], string),
          destinationPort: json |> at(["packet", "destination_port"], string),
          destinationChannel: json |> at(["packet", "destination_channel"], string),
          timeoutHeight: json |> at(["packet", "timeout_height"], int),
          chainID: "band-consumer",
        },
        nextSequenceReceive: json |> at(["packet", "next_sequence_receive"], int),
      };
  };

  module ConnectionCommon = {
    type t = {
      chainID: string,
      connectionID: string,
    };
    let decode = json =>
      JsonUtils.Decode.{
        chainID: "band-consumer",
        connectionID: json |> field("connection_id", string),
      };
  };

  module ConnectionOpenInit = {
    type t = {
      signer: Address.t,
      common: ConnectionCommon.t,
      clientID: string,
    };
    let decode = json =>
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ConnectionCommon.decode,
        clientID: json |> field("client_id", string),
      };
  };

  module ConnectionOpenTry = {
    type t = {
      signer: Address.t,
      common: ConnectionCommon.t,
      clientID: string,
    };
    let decode = json =>
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        clientID: json |> field("client_id", string),
        common: json |> ConnectionCommon.decode,
      };
  };

  module ConnectionOpenAck = {
    type t = {
      signer: Address.t,
      common: ConnectionCommon.t,
    };
    let decode = json =>
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ConnectionCommon.decode,
      };
  };

  module ConnectionOpenConfirm = {
    type t = {
      signer: Address.t,
      common: ConnectionCommon.t,
    };
    let decode = json =>
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ConnectionCommon.decode,
      };
  };

  module ChannelCommon = {
    type t = {
      chainID: string,
      portID: string,
      channelID: string,
    };

    let decode = json => {
      JsonUtils.Decode.{
        chainID: "band-consumer",
        portID: json |> field("port_id", string),
        channelID: json |> field("channel_id", string),
      };
    };
  };

  module ChannelOpenInit = {
    type t = {
      signer: Address.t,
      common: ChannelCommon.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ChannelCommon.decode,
      };
    };
  };

  module ChannelOpenTry = {
    type t = {
      signer: Address.t,
      common: ChannelCommon.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ChannelCommon.decode,
      };
    };
  };

  module ChannelOpenAck = {
    type t = {
      signer: Address.t,
      common: ChannelCommon.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ChannelCommon.decode,
      };
    };
  };

  module ChannelOpenConfirm = {
    type t = {
      signer: Address.t,
      common: ChannelCommon.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ChannelCommon.decode,
      };
    };
  };

  module ChannelCloseInit = {
    type t = {
      signer: Address.t,
      common: ChannelCommon.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ChannelCommon.decode,
      };
    };
  };

  module ChannelCloseConfirm = {
    type t = {
      signer: Address.t,
      common: ChannelCommon.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        signer: json |> field("signer", string) |> Address.fromBech32,
        common: json |> ChannelCommon.decode,
      };
    };
  };

  module Delegate = {
    type success_t = {
      validatorAddress: Address.t,
      delegatorAddress: Address.t,
      amount: Coin.t,
      moniker: string,
      identity: string,
    };
    type fail_t = {
      validatorAddress: Address.t,
      delegatorAddress: Address.t,
      amount: Coin.t,
    };

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        amount: json |> at(["msg", "amount"], Coin.decodeCoin),
        moniker: json |> at(["extra", "moniker"], string),
        identity: json |> at(["extra", "identity"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        amount: json |> at(["msg", "amount"], Coin.decodeCoin),
      };
    };
  };

  module Undelegate = {
    type success_t = {
      validatorAddress: Address.t,
      delegatorAddress: Address.t,
      amount: Coin.t,
      moniker: string,
      identity: string,
    };
    type fail_t = {
      validatorAddress: Address.t,
      delegatorAddress: Address.t,
      amount: Coin.t,
    };

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        amount: json |> at(["msg", "amount"], Coin.decodeCoin),
        moniker: json |> at(["extra", "moniker"], string),
        identity: json |> at(["extra", "identity"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        amount: json |> at(["msg", "amount"], Coin.decodeCoin),
      };
    };
  };

  module Redelegate = {
    type success_t = {
      validatorSourceAddress: Address.t,
      validatorDestinationAddress: Address.t,
      delegatorAddress: Address.t,
      amount: Coin.t,
      monikerSource: string,
      monikerDestination: string,
      identitySource: string,
      identityDestination: string,
    };

    type fail_t = {
      validatorSourceAddress: Address.t,
      validatorDestinationAddress: Address.t,
      delegatorAddress: Address.t,
      amount: Coin.t,
    };
    let decodeSuccess = json => {
      JsonUtils.Decode.{
        validatorSourceAddress:
          json |> at(["msg", "validator_src_address"], string) |> Address.fromBech32,
        validatorDestinationAddress:
          json |> at(["msg", "validator_dst_address"], string) |> Address.fromBech32,
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        amount: json |> at(["msg", "amount"], Coin.decodeCoin),
        monikerSource: json |> at(["extra", "val_src_moniker"], string),
        monikerDestination: json |> at(["extra", "val_dst_moniker"], string),
        identitySource: json |> at(["extra", "val_src_identity"], string),
        identityDestination: json |> at(["extra", "val_dst_identity"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        validatorSourceAddress:
          json |> at(["msg", "validator_src_address"], string) |> Address.fromBech32,
        validatorDestinationAddress:
          json |> at(["msg", "validator_dst_address"], string) |> Address.fromBech32,
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        amount: json |> at(["msg", "amount"], Coin.decodeCoin),
      };
    };
  };

  module WithdrawReward = {
    type success_t = {
      validatorAddress: Address.t,
      delegatorAddress: Address.t,
      amount: list(Coin.t),
      moniker: string,
      identity: string,
    };
    type fail_t = {
      validatorAddress: Address.t,
      delegatorAddress: Address.t,
    };

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        amount: json |> at(["extra", "reward_amount"], string) |> GraphQLParser.coins,
        moniker: json |> at(["extra", "moniker"], string),
        identity: json |> at(["extra", "moniker"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      };
    };
  };

  module Unjail = {
    type success_t = {
      address: Address.t,
      identity: string,
      moniker: string,
    };

    type fail_t = {address: Address.t};

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        address: json |> at(["msg", "address"], string) |> Address.fromBech32,
        identity: json |> at(["extra", "identity"], string),
        moniker: json |> at(["extra", "moniker"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{address: json |> at(["msg", "address"], string) |> Address.fromBech32};
    };
  };

  module SetWithdrawAddress = {
    type t = {
      delegatorAddress: Address.t,
      withdrawAddress: Address.t,
    };
    let decode = json => {
      JsonUtils.Decode.{
        delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
        withdrawAddress: json |> at(["msg", "withdraw_address"], string) |> Address.fromBech32,
      };
    };
  };

  module SubmitProposal = {
    type success_t = {
      proposer: Address.t,
      title: string,
      description: string,
      initialDeposit: list(Coin.t),
      proposalID: ID.Proposal.t,
    };

    type fail_t = {
      proposer: Address.t,
      title: string,
      description: string,
      initialDeposit: list(Coin.t),
    };

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        proposer: json |> at(["msg", "proposer"], string) |> Address.fromBech32,
        title: json |> at(["msg", "content", "title"], string),
        description: json |> at(["msg", "content", "description"], string),
        initialDeposit: json |> at(["msg", "initial_deposit"], list(Coin.decodeCoin)),
        proposalID: json |> at(["extra", "proposal_id"], ID.Proposal.fromJson),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        proposer: json |> at(["msg", "proposer"], string) |> Address.fromBech32,
        title: json |> at(["msg", "content", "title"], string),
        description: json |> at(["msg", "content", "description"], string),
        initialDeposit: json |> at(["msg", "initial_deposit"], list(Coin.decodeCoin)),
      };
    };
  };

  module Deposit = {
    type success_t = {
      depositor: Address.t,
      proposalID: ID.Proposal.t,
      amount: list(Coin.t),
      title: string,
    };

    type fail_t = {
      depositor: Address.t,
      proposalID: ID.Proposal.t,
      amount: list(Coin.t),
    };

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        depositor: json |> at(["msg", "depositor"], string) |> Address.fromBech32,
        proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
        amount: json |> at(["msg", "amount"], list(Coin.decodeCoin)),
        title: json |> at(["extra", "title"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        depositor: json |> at(["msg", "depositor"], string) |> Address.fromBech32,
        proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
        amount: json |> at(["msg", "amount"], list(Coin.decodeCoin)),
      };
    };
  };
  module Vote = {
    type success_t = {
      voterAddress: Address.t,
      proposalID: ID.Proposal.t,
      option: string,
      title: string,
    };

    type fail_t = {
      voterAddress: Address.t,
      proposalID: ID.Proposal.t,
      option: string,
    };

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        voterAddress: json |> at(["msg", "voter"], string) |> Address.fromBech32,
        proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
        option: json |> at(["msg", "option"], string),
        title: json |> at(["extra", "title"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        voterAddress: json |> at(["msg", "voter"], string) |> Address.fromBech32,
        proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
        option: json |> at(["msg", "option"], string),
      };
    };
  };
  module WithdrawCommission = {
    type success_t = {
      validatorAddress: Address.t,
      amount: list(Coin.t),
      moniker: string,
      identity: string,
    };
    type fail_t = {validatorAddress: Address.t};

    let decodeSuccess = json => {
      JsonUtils.Decode.{
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
        amount: json |> at(["extra", "commission_amount"], string) |> GraphQLParser.coins,
        moniker: json |> at(["extra", "moniker"], string),
        identity: json |> at(["extra", "identity"], string),
      };
    };

    let decodeFail = json => {
      JsonUtils.Decode.{
        validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      };
    };
  };

  module MultiSend = {
    type send_tx_t = {
      address: Address.t,
      coins: list(Coin.t),
    };
    type t = {
      inputs: list(send_tx_t),
      outputs: list(send_tx_t),
    };
    let decodeSendTx = json => {
      JsonUtils.Decode.{
        address: json |> field("address", string) |> Address.fromBech32,
        coins: json |> field("coins", list(Coin.decodeCoin)),
      };
    };
    let decode = json => {
      JsonUtils.Decode.{
        inputs: json |> at(["msg", "inputs"], list(decodeSendTx)),
        outputs: json |> at(["msg", "outputs"], list(decodeSendTx)),
      };
    };
  };

  module Activate = {
    type t = {validatorAddress: Address.t};

    let decode = json => {
      JsonUtils.Decode.{
        validatorAddress: json |> at(["msg", "validator"], string) |> Address.fromBech32,
      };
    };
  };

  type t =
    | SendMsgSuccess(Send.t)
    | SendMsgFail(Send.t)
    | ReceiveMsg(Receive.t)
    | CreateDataSourceMsgSuccess(CreateDataSource.success_t)
    | CreateDataSourceMsgFail(CreateDataSource.fail_t)
    | EditDataSourceMsgSuccess(EditDataSource.t)
    | EditDataSourceMsgFail(EditDataSource.t)
    | CreateOracleScriptMsgSuccess(CreateOracleScript.success_t)
    | CreateOracleScriptMsgFail(CreateOracleScript.fail_t)
    | EditOracleScriptMsgSuccess(EditOracleScript.t)
    | EditOracleScriptMsgFail(EditOracleScript.t)
    | RequestMsgSuccess(Request.success_t)
    | RequestMsgFail(Request.fail_t)
    | ReportMsgSuccess(Report.t)
    | ReportMsgFail(Report.t)
    | AddReporterMsgSuccess(AddReporter.success_t)
    | AddReporterMsgFail(AddReporter.fail_t)
    | RemoveReporterMsgSuccess(RemoveReporter.success_t)
    | RemoveReporterMsgFail(RemoveReporter.fail_t)
    | CreateValidatorMsgSuccess(CreateValidator.t)
    | CreateValidatorMsgFail(CreateValidator.t)
    | EditValidatorMsgSuccess(EditValidator.t)
    | EditValidatorMsgFail(EditValidator.t)
    | CreateClientMsg(CreateClient.t)
    | UpdateClientMsg(UpdateClient.t)
    | SubmitClientMisbehaviourMsg(SubmitClientMisbehaviour.t)
    | ConnectionOpenInitMsg(ConnectionOpenInit.t)
    | ConnectionOpenTryMsg(ConnectionOpenTry.t)
    | ConnectionOpenAckMsg(ConnectionOpenAck.t)
    | ConnectionOpenConfirmMsg(ConnectionOpenConfirm.t)
    | ChannelOpenInitMsg(ChannelOpenInit.t)
    | ChannelOpenTryMsg(ChannelOpenTry.t)
    | ChannelOpenAckMsg(ChannelOpenAck.t)
    | ChannelOpenConfirmMsg(ChannelOpenConfirm.t)
    | ChannelCloseInitMsg(ChannelCloseInit.t)
    | ChannelCloseConfirmMsg(ChannelCloseConfirm.t)
    | PacketMsg(Packet.t)
    | AcknowledgementMsg(Acknowledgement.t)
    | TimeoutMsg(Timeout.t)
    | DelegateMsgSuccess(Delegate.success_t)
    | DelegateMsgFail(Delegate.fail_t)
    | UndelegateMsgSuccess(Undelegate.success_t)
    | UndelegateMsgFail(Undelegate.fail_t)
    | RedelegateMsgSuccess(Redelegate.success_t)
    | RedelegateMsgFail(Redelegate.fail_t)
    | WithdrawRewardMsgSuccess(WithdrawReward.success_t)
    | WithdrawRewardMsgFail(WithdrawReward.fail_t)
    | UnjailMsgSuccess(Unjail.success_t)
    | UnjailMsgFail(Unjail.fail_t)
    | SetWithdrawAddressMsgSuccess(SetWithdrawAddress.t)
    | SetWithdrawAddressMsgFail(SetWithdrawAddress.t)
    | SubmitProposalMsgSuccess(SubmitProposal.success_t)
    | SubmitProposalMsgFail(SubmitProposal.fail_t)
    | DepositMsgSuccess(Deposit.success_t)
    | DepositMsgFail(Deposit.fail_t)
    | VoteMsgSuccess(Vote.success_t)
    | VoteMsgFail(Vote.fail_t)
    | WithdrawCommissionMsgSuccess(WithdrawCommission.success_t)
    | WithdrawCommissionMsgFail(WithdrawCommission.fail_t)
    | MultiSendMsgSuccess(MultiSend.t)
    | MultiSendMsgFail(MultiSend.t)
    | ActivateMsgSuccess(Activate.t)
    | ActivateMsgFail(Activate.t)
    | UnknownMsg;

  let getCreator = msg => {
    switch (msg) {
    | ReceiveMsg(receive) => receive.fromAddress
    | SendMsgSuccess(send)
    | SendMsgFail(send) => send.fromAddress
    | CreateDataSourceMsgSuccess(dataSource) => dataSource.sender
    | CreateDataSourceMsgFail(dataSource) => dataSource.sender
    | EditDataSourceMsgSuccess(dataSource) => dataSource.sender
    | EditDataSourceMsgFail(dataSource) => dataSource.sender
    | CreateOracleScriptMsgSuccess(oracleScript) => oracleScript.sender
    | CreateOracleScriptMsgFail(oracleScript) => oracleScript.sender
    | EditOracleScriptMsgSuccess(oracleScript) => oracleScript.sender
    | EditOracleScriptMsgFail(oracleScript) => oracleScript.sender
    | RequestMsgSuccess(request) => request.sender
    | RequestMsgFail(request) => request.sender
    | ReportMsgSuccess(report)
    | ReportMsgFail(report) => report.reporter
    | AddReporterMsgSuccess(address) => address.validator
    | AddReporterMsgFail(address) => address.validator
    | RemoveReporterMsgSuccess(address) => address.validator
    | RemoveReporterMsgFail(address) => address.validator
    | CreateValidatorMsgSuccess(validator)
    | CreateValidatorMsgFail(validator) => validator.delegatorAddress
    | EditValidatorMsgSuccess(validator)
    | EditValidatorMsgFail(validator) => validator.sender
    | DelegateMsgSuccess(delegation) => delegation.delegatorAddress
    | DelegateMsgFail(delegation) => delegation.delegatorAddress
    | UndelegateMsgSuccess(delegation) => delegation.delegatorAddress
    | UndelegateMsgFail(delegation) => delegation.delegatorAddress
    | RedelegateMsgSuccess(delegation) => delegation.delegatorAddress
    | RedelegateMsgFail(delegation) => delegation.delegatorAddress
    | WithdrawRewardMsgSuccess(withdrawal) => withdrawal.delegatorAddress
    | WithdrawRewardMsgFail(withdrawal) => withdrawal.delegatorAddress
    | UnjailMsgSuccess(validator) => validator.address
    | UnjailMsgFail(validator) => validator.address
    | SetWithdrawAddressMsgSuccess(set)
    | SetWithdrawAddressMsgFail(set) => set.delegatorAddress
    | SubmitProposalMsgSuccess(proposal) => proposal.proposer
    | SubmitProposalMsgFail(proposal) => proposal.proposer
    | DepositMsgSuccess(deposit) => deposit.depositor
    | DepositMsgFail(deposit) => deposit.depositor
    | VoteMsgSuccess(vote) => vote.voterAddress
    | VoteMsgFail(vote) => vote.voterAddress
    | WithdrawCommissionMsgSuccess(withdrawal) => withdrawal.validatorAddress
    | WithdrawCommissionMsgFail(withdrawal) => withdrawal.validatorAddress
    | MultiSendMsgSuccess(tx)
    | MultiSendMsgFail(tx) =>
      let firstInput = tx.inputs |> Belt_List.getExn(_, 0);
      firstInput.address;
    | ActivateMsgSuccess(activator)
    | ActivateMsgFail(activator) => activator.validatorAddress
    //TODO: Revisit IBC msg
    | CreateClientMsg(client) => client.address
    | UpdateClientMsg(client) => client.address
    | SubmitClientMisbehaviourMsg(client) => client.address
    | ConnectionOpenInitMsg(connection) => connection.signer
    | ConnectionOpenTryMsg(connection) => connection.signer
    | ConnectionOpenAckMsg(connection) => connection.signer
    | ConnectionOpenConfirmMsg(connection) => connection.signer
    | ChannelOpenInitMsg(channel) => channel.signer
    | ChannelOpenTryMsg(channel) => channel.signer
    | ChannelOpenAckMsg(channel) => channel.signer
    | ChannelOpenConfirmMsg(channel) => channel.signer
    | ChannelCloseInitMsg(channel) => channel.signer
    | ChannelCloseConfirmMsg(channel) => channel.signer
    | PacketMsg(packet) => packet.sender
    | AcknowledgementMsg(ack) => ack.sender
    | TimeoutMsg(timeout) => timeout.sender
    | _ => "" |> Address.fromHex
    };
  };

  type badge_theme_t = {
    name: string,
    category: msg_cat_t,
  };

  let getBadge = badgeVariant => {
    switch (badgeVariant) {
    | SendBadge => {name: "Send", category: TokenMsg}
    | ReceiveBadge => {name: "Receive", category: TokenMsg}
    | CreateDataSourceBadge => {name: "Create Data Source", category: DataMsg}
    | EditDataSourceBadge => {name: "Edit Data Source", category: DataMsg}
    | CreateOracleScriptBadge => {name: "Create Oracle Script", category: DataMsg}
    | EditOracleScriptBadge => {name: "Edit Oracle Script", category: DataMsg}
    | RequestBadge => {name: "Request", category: DataMsg}
    | ReportBadge => {name: "Report", category: DataMsg}
    | AddReporterBadge => {name: "Add Reporter", category: ValidatorMsg}
    | RemoveReporterBadge => {name: "Remove Reporter", category: ValidatorMsg}
    | CreateValidatorBadge => {name: "Create Validator", category: ValidatorMsg}
    | EditValidatorBadge => {name: "Edit Validator", category: ValidatorMsg}
    | DelegateBadge => {name: "Delegate", category: TokenMsg}
    | UndelegateBadge => {name: "Undelegate", category: TokenMsg}
    | RedelegateBadge => {name: "Redelegate", category: TokenMsg}
    | VoteBadge => {name: "Vote", category: ProposalMsg}
    | WithdrawRewardBadge => {name: "Withdraw Reward", category: TokenMsg}
    | UnjailBadge => {name: "Unjail", category: ValidatorMsg}
    | SetWithdrawAddressBadge => {name: "Set Withdraw Address", category: ValidatorMsg}
    | SubmitProposalBadge => {name: "Submit Proposal", category: ProposalMsg}
    | DepositBadge => {name: "Deposit", category: ProposalMsg}
    | WithdrawCommissionBadge => {name: "Withdraw Commission", category: TokenMsg}
    | MultiSendBadge => {name: "Multi Send", category: TokenMsg}
    | ActivateBadge => {name: "Activate", category: ValidatorMsg}
    | UnknownBadge => {name: "Unknown", category: TokenMsg}
    //TODO: Revisit IBC msg
    | CreateClientBadge => {name: "Create Client", category: TokenMsg}
    | UpdateClientBadge => {name: "Update Client", category: TokenMsg}
    | SubmitClientMisbehaviourBadge => {name: "Submit Client Misbehaviour", category: TokenMsg}
    | ConnectionOpenInitBadge => {name: "Connection Open Init", category: TokenMsg}
    | ConnectionOpenTryBadge => {name: "Connection Open Try", category: TokenMsg}
    | ConnectionOpenAckBadge => {name: "Connection Open Ack", category: TokenMsg}
    | ConnectionOpenConfirmBadge => {name: "Connection Open Confirm", category: TokenMsg}
    | ChannelOpenInitBadge => {name: "Channel Open Init", category: TokenMsg}
    | ChannelOpenTryBadge => {name: "Channel Open Try", category: TokenMsg}
    | ChannelOpenAckBadge => {name: "Channel Open Ack", category: TokenMsg}
    | ChannelOpenConfirmBadge => {name: "Channel Open Confirm", category: TokenMsg}
    | ChannelCloseInitBadge => {name: "Channel Close Init", category: TokenMsg}
    | ChannelCloseConfirmBadge => {name: "Channel Close Confirm", category: TokenMsg}
    | PacketBadge => {name: "Packet", category: TokenMsg}
    | AcknowledgementBadge => {name: "Acknowledgement", category: TokenMsg}
    | TimeoutBadge => {name: "Timeout", category: TokenMsg}
    };
  };

  let getBadgeTheme = msg => {
    switch (msg) {
    | SendMsgSuccess(_)
    | SendMsgFail(_) => getBadge(SendBadge)
    | ReceiveMsg(_) => getBadge(ReceiveBadge)
    | CreateDataSourceMsgSuccess(_)
    | CreateDataSourceMsgFail(_) => getBadge(CreateDataSourceBadge)
    | EditDataSourceMsgSuccess(_)
    | EditDataSourceMsgFail(_) => getBadge(EditDataSourceBadge)
    | CreateOracleScriptMsgSuccess(_)
    | CreateOracleScriptMsgFail(_) => getBadge(CreateOracleScriptBadge)
    | EditOracleScriptMsgSuccess(_)
    | EditOracleScriptMsgFail(_) => getBadge(EditOracleScriptBadge)
    | RequestMsgSuccess(_)
    | RequestMsgFail(_) => getBadge(RequestBadge)
    | ReportMsgSuccess(_)
    | ReportMsgFail(_) => getBadge(ReportBadge)
    | AddReporterMsgSuccess(_)
    | AddReporterMsgFail(_) => getBadge(AddReporterBadge)
    | RemoveReporterMsgSuccess(_)
    | RemoveReporterMsgFail(_) => getBadge(RemoveReporterBadge)
    | CreateValidatorMsgSuccess(_)
    | CreateValidatorMsgFail(_) => getBadge(CreateValidatorBadge)
    | EditValidatorMsgSuccess(_)
    | EditValidatorMsgFail(_) => getBadge(EditValidatorBadge)
    | DelegateMsgSuccess(_)
    | DelegateMsgFail(_) => getBadge(DelegateBadge)
    | UndelegateMsgSuccess(_)
    | UndelegateMsgFail(_) => getBadge(UndelegateBadge)
    | RedelegateMsgSuccess(_)
    | RedelegateMsgFail(_) => getBadge(RedelegateBadge)
    | VoteMsgSuccess(_)
    | VoteMsgFail(_) => getBadge(VoteBadge)
    | WithdrawRewardMsgSuccess(_)
    | WithdrawRewardMsgFail(_) => getBadge(WithdrawRewardBadge)
    | UnjailMsgSuccess(_)
    | UnjailMsgFail(_) => getBadge(UnjailBadge)
    | SetWithdrawAddressMsgSuccess(_)
    | SetWithdrawAddressMsgFail(_) => getBadge(SetWithdrawAddressBadge)
    | SubmitProposalMsgSuccess(_)
    | SubmitProposalMsgFail(_) => getBadge(SubmitProposalBadge)
    | DepositMsgSuccess(_)
    | DepositMsgFail(_) => getBadge(DepositBadge)
    | WithdrawCommissionMsgSuccess(_)
    | WithdrawCommissionMsgFail(_) => getBadge(WithdrawCommissionBadge)
    | MultiSendMsgSuccess(_) => getBadge(MultiSendBadge)
    | MultiSendMsgFail(_) => getBadge(MultiSendBadge)
    | ActivateMsgSuccess(_) => getBadge(ActivateBadge)
    | ActivateMsgFail(_) => getBadge(ActivateBadge)
    | UnknownMsg => getBadge(UnknownBadge)
    //TODO: Revisit IBC msg
    | CreateClientMsg(_) => getBadge(CreateClientBadge)
    | UpdateClientMsg(_) => getBadge(UpdateClientBadge)
    | SubmitClientMisbehaviourMsg(_) => getBadge(SubmitClientMisbehaviourBadge)
    | ConnectionOpenInitMsg(_) => getBadge(ConnectionOpenInitBadge)
    | ConnectionOpenTryMsg(_) => getBadge(ConnectionOpenTryBadge)
    | ConnectionOpenAckMsg(_) => getBadge(ConnectionOpenAckBadge)
    | ConnectionOpenConfirmMsg(_) => getBadge(ConnectionOpenConfirmBadge)
    | ChannelOpenInitMsg(_) => getBadge(ChannelOpenInitBadge)
    | ChannelOpenTryMsg(_) => getBadge(ChannelOpenTryBadge)
    | ChannelOpenAckMsg(_) => getBadge(ChannelOpenAckBadge)
    | ChannelOpenConfirmMsg(_) => getBadge(ChannelOpenConfirmBadge)
    | ChannelCloseInitMsg(_) => getBadge(ChannelCloseInitBadge)
    | ChannelCloseConfirmMsg(_) => getBadge(ChannelCloseConfirmBadge)
    | PacketMsg(_) => getBadge(PacketBadge)
    | AcknowledgementMsg(_) => getBadge(AcknowledgementBadge)
    | TimeoutMsg(_) => getBadge(TimeoutBadge)
    };
  };

  let decodeAction = json => {
    JsonUtils.Decode.(
      switch (json |> field("type", string) |> getBadgeVariantFromString) {
      | SendBadge => SendMsgSuccess(json |> Send.decode)
      | ReceiveBadge => raise(Not_found)
      | CreateDataSourceBadge =>
        CreateDataSourceMsgSuccess(json |> CreateDataSource.decodeSuccess)
      | EditDataSourceBadge => EditDataSourceMsgSuccess(json |> EditDataSource.decode)
      | CreateOracleScriptBadge =>
        CreateOracleScriptMsgSuccess(json |> CreateOracleScript.decodeSuccess)
      | EditOracleScriptBadge => EditOracleScriptMsgSuccess(json |> EditOracleScript.decode)
      | RequestBadge => RequestMsgSuccess(json |> Request.decodeSuccess)
      | ReportBadge => ReportMsgSuccess(json |> Report.decode)
      | AddReporterBadge => AddReporterMsgSuccess(json |> AddReporter.decodeSuccess)
      | RemoveReporterBadge => RemoveReporterMsgSuccess(json |> RemoveReporter.decodeSuccess)
      | CreateValidatorBadge => CreateValidatorMsgSuccess(json |> CreateValidator.decode)
      | EditValidatorBadge => EditValidatorMsgSuccess(json |> EditValidator.decode)
      | DelegateBadge => DelegateMsgSuccess(json |> Delegate.decodeSuccess)
      | UndelegateBadge => UndelegateMsgSuccess(json |> Undelegate.decodeSuccess)
      | RedelegateBadge => RedelegateMsgSuccess(json |> Redelegate.decodeSuccess)
      | WithdrawRewardBadge => WithdrawRewardMsgSuccess(json |> WithdrawReward.decodeSuccess)
      | UnjailBadge => UnjailMsgSuccess(json |> Unjail.decodeSuccess)
      | SetWithdrawAddressBadge => SetWithdrawAddressMsgSuccess(json |> SetWithdrawAddress.decode)
      | SubmitProposalBadge => SubmitProposalMsgSuccess(json |> SubmitProposal.decodeSuccess)
      | DepositBadge => DepositMsgSuccess(json |> Deposit.decodeSuccess)
      | VoteBadge => VoteMsgSuccess(json |> Vote.decodeSuccess)
      | WithdrawCommissionBadge =>
        WithdrawCommissionMsgSuccess(json |> WithdrawCommission.decodeSuccess)
      | MultiSendBadge => MultiSendMsgSuccess(json |> MultiSend.decode)
      | ActivateBadge => ActivateMsgSuccess(json |> Activate.decode)
      | UnknownBadge => UnknownMsg
      //TODO: Revisit IBC msg
      | CreateClientBadge => CreateClientMsg(json |> CreateClient.decode)
      | UpdateClientBadge => UpdateClientMsg(json |> UpdateClient.decode)
      | SubmitClientMisbehaviourBadge =>
        SubmitClientMisbehaviourMsg(json |> SubmitClientMisbehaviour.decode)
      | ConnectionOpenInitBadge => ConnectionOpenInitMsg(json |> ConnectionOpenInit.decode)
      | ConnectionOpenTryBadge => ConnectionOpenTryMsg(json |> ConnectionOpenTry.decode)
      | ConnectionOpenAckBadge => ConnectionOpenAckMsg(json |> ConnectionOpenAck.decode)
      | ConnectionOpenConfirmBadge =>
        ConnectionOpenConfirmMsg(json |> ConnectionOpenConfirm.decode)
      | ChannelOpenInitBadge => ChannelOpenInitMsg(json |> ChannelOpenInit.decode)
      | ChannelOpenTryBadge => ChannelOpenTryMsg(json |> ChannelOpenTry.decode)
      | ChannelOpenAckBadge => ChannelOpenAckMsg(json |> ChannelOpenAck.decode)
      | ChannelOpenConfirmBadge => ChannelOpenConfirmMsg(json |> ChannelOpenConfirm.decode)
      | ChannelCloseInitBadge => ChannelCloseInitMsg(json |> ChannelCloseInit.decode)
      | ChannelCloseConfirmBadge => ChannelCloseConfirmMsg(json |> ChannelCloseConfirm.decode)
      | PacketBadge => PacketMsg(json |> Packet.decode)
      | TimeoutBadge => TimeoutMsg(json |> Timeout.decode)
      // TODO: handle case correctly
      | AcknowledgementBadge => AcknowledgementMsg(json |> Acknowledgement.decode)
      }
    );
  };

  let decodeFailAction = json => {
    JsonUtils.Decode.(
      switch (json |> field("type", string) |> getBadgeVariantFromString) {
      | SendBadge => SendMsgFail(json |> Send.decode)
      | ReceiveBadge => raise(Not_found)
      | CreateDataSourceBadge => CreateDataSourceMsgFail(json |> CreateDataSource.decodeFail)
      | EditDataSourceBadge => EditDataSourceMsgFail(json |> EditDataSource.decode)
      | CreateOracleScriptBadge =>
        CreateOracleScriptMsgFail(json |> CreateOracleScript.decodeFail)
      | EditOracleScriptBadge => EditOracleScriptMsgFail(json |> EditOracleScript.decode)
      | RequestBadge => RequestMsgFail(json |> Request.decodeFail)
      | ReportBadge => ReportMsgFail(json |> Report.decode)
      | AddReporterBadge => AddReporterMsgFail(json |> AddReporter.decodeFail)
      | RemoveReporterBadge => RemoveReporterMsgFail(json |> RemoveReporter.decodeFail)
      | CreateValidatorBadge => CreateValidatorMsgFail(json |> CreateValidator.decode)
      | EditValidatorBadge => EditValidatorMsgFail(json |> EditValidator.decode)
      | DelegateBadge => DelegateMsgFail(json |> Delegate.decodeFail)
      | UndelegateBadge => UndelegateMsgFail(json |> Undelegate.decodeFail)
      | RedelegateBadge => RedelegateMsgFail(json |> Redelegate.decodeFail)
      | WithdrawRewardBadge => WithdrawRewardMsgFail(json |> WithdrawReward.decodeFail)
      | UnjailBadge => UnjailMsgFail(json |> Unjail.decodeFail)
      | SetWithdrawAddressBadge => SetWithdrawAddressMsgFail(json |> SetWithdrawAddress.decode)
      | SubmitProposalBadge => SubmitProposalMsgFail(json |> SubmitProposal.decodeFail)
      | DepositBadge => DepositMsgFail(json |> Deposit.decodeFail)
      | VoteBadge => VoteMsgFail(json |> Vote.decodeFail)
      | WithdrawCommissionBadge =>
        WithdrawCommissionMsgFail(json |> WithdrawCommission.decodeFail)
      | MultiSendBadge => MultiSendMsgFail(json |> MultiSend.decode)
      | ActivateBadge => ActivateMsgFail(json |> Activate.decode)
      | UnknownBadge => UnknownMsg
      | _ => UnknownMsg
      }
    );
  };
};

type block_t = {timestamp: MomentRe.Moment.t};

type t = {
  id: int,
  txHash: Hash.t,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  sender: Address.t,
  timestamp: MomentRe.Moment.t,
  messages: list(Msg.t),
  memo: string,
  errMsg: string,
};

type internal_t = {
  id: int,
  txHash: Hash.t,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  sender: Address.t,
  block: block_t,
  messages: Js.Json.t,
  memo: string,
  errMsg: option(string),
};

type account_transaction_t = {transaction: internal_t};

module Mini = {
  type block_t = {timestamp: MomentRe.Moment.t};
  type t = {
    hash: Hash.t,
    blockHeight: ID.Block.t,
    block: block_t,
    gasFee: list(Coin.t),
  };
};

let toExternal =
    (
      {
        id,
        txHash,
        blockHeight,
        success,
        gasFee,
        gasLimit,
        gasUsed,
        sender,
        memo,
        block,
        messages,
        errMsg,
      },
    ) => {
  id,
  txHash,
  blockHeight,
  success,
  gasFee,
  gasLimit,
  gasUsed,
  sender,
  memo,
  timestamp: block.timestamp,
  messages: {
    let msg = messages |> Js.Json.decodeArray |> Belt.Option.getExn |> Belt.List.fromArray;
    msg->Belt.List.map(success ? Msg.decodeAction : Msg.decodeFailAction);
  },
  errMsg: errMsg->Belt.Option.getWithDefault(""),
};

module SingleConfig = [%graphql
  {|
  subscription Transaction($tx_hash: bytea!) {
    transactions_by_pk(hash: $tx_hash) @bsRecord {
      id
      txHash: hash @bsDecoder(fn: "GraphQLParser.hash")
      blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
      success
      memo
      gasFee: gas_fee @bsDecoder(fn: "GraphQLParser.coins")
      gasLimit: gas_limit
      gasUsed: gas_used
      sender  @bsDecoder(fn: "Address.fromBech32")
      messages
      errMsg: err_msg
      block @bsRecord {
        timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
      }
    }
  },
|}
];

module MultiConfig = [%graphql
  {|
  subscription Transactions($limit: Int!, $offset: Int!) {
    transactions(offset: $offset, limit: $limit, order_by: {id: desc}) @bsRecord {
      id
      txHash: hash @bsDecoder(fn: "GraphQLParser.hash")
      blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
      success
      memo
      gasFee: gas_fee @bsDecoder(fn: "GraphQLParser.coins")
      gasLimit: gas_limit
      gasUsed: gas_used
      sender  @bsDecoder(fn: "Address.fromBech32")
      messages
      errMsg: err_msg
      block @bsRecord {
        timestamp  @bsDecoder(fn: "GraphQLParser.timestamp")
      }
    }
  }
|}
];

module MultiByHeightConfig = [%graphql
  {|
  subscription TransactionsByHeight($height: Int!, $limit: Int!, $offset: Int!) {
    transactions(where: {block_height: {_eq: $height}}, offset: $offset, limit: $limit, order_by: {id: desc}) @bsRecord {
      id
      txHash: hash @bsDecoder(fn: "GraphQLParser.hash")
      blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
      success
      memo
      gasFee: gas_fee @bsDecoder(fn: "GraphQLParser.coins")
      gasLimit: gas_limit
      gasUsed: gas_used
      sender  @bsDecoder(fn: "Address.fromBech32")
      messages
      errMsg: err_msg
      block @bsRecord {
        timestamp  @bsDecoder(fn: "GraphQLParser.timestamp")
      }
    }
  }
|}
];

module MultiBySenderConfig = [%graphql
  {|
  subscription TransactionsBySender($sender: String!, $limit: Int!, $offset: Int!) {
    accounts_by_pk(address: $sender) {
      account_transactions(offset: $offset, limit: $limit, order_by: {transaction_id: desc}) @bsRecord{
        transaction @bsRecord {
          id
          txHash: hash @bsDecoder(fn: "GraphQLParser.hash")
          blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
          success
          memo
          gasFee: gas_fee @bsDecoder(fn: "GraphQLParser.coins")
          gasLimit: gas_limit
          gasUsed: gas_used
          sender  @bsDecoder(fn: "Address.fromBech32")
          messages
          errMsg: err_msg
          block @bsRecord {
            timestamp  @bsDecoder(fn: "GraphQLParser.timestamp")
          }
        }
      }
    }

  }
|}
];

module TxCountConfig = [%graphql
  {|
  subscription TransactionsCount {
    transactions_aggregate {
      aggregate {
        count @bsDecoder(fn: "Belt_Option.getExn")
      }
    }
  }
|}
];

module TxCountBySenderConfig = [%graphql
  {|
  subscription TransactionsCountBySender($sender: String!) {
    accounts_by_pk(address: $sender) {
      account_transactions_aggregate {
        aggregate {
          count @bsDecoder(fn: "Belt_Option.getExn")
        }
      }
    }
  }
|}
];

let get = txHash => {
  let (result, _) =
    ApolloHooks.useSubscription(
      SingleConfig.definition,
      ~variables=
        SingleConfig.makeVariables(
          ~tx_hash=txHash |> Hash.toHex |> (x => "\\x" ++ x) |> Js.Json.string,
          (),
        ),
    );
  let%Sub x = result;
  switch (x##transactions_by_pk) {
  | Some(data) => Sub.resolve(data |> toExternal)
  | None => NoData
  };
};

let getList = (~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiConfig.definition,
      ~variables=MultiConfig.makeVariables(~limit=pageSize, ~offset, ()),
    );
  result |> Sub.map(_, x => x##transactions->Belt_Array.map(toExternal));
};

let getListBySender = (sender, ~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiBySenderConfig.definition,
      ~variables=
        MultiBySenderConfig.makeVariables(
          ~sender=sender |> Address.toBech32,
          ~limit=pageSize,
          ~offset,
          (),
        ),
    );
  result
  |> Sub.map(_, x => {
       switch (x##accounts_by_pk) {
       | Some(x') =>
         x'##account_transactions->Belt_Array.map(({transaction}) => transaction->toExternal)
       | None => [||]
       }
     });
};

let getListByBlockHeight = (height, ~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiByHeightConfig.definition,
      ~variables=
        MultiByHeightConfig.makeVariables(
          ~height=height |> ID.Block.toInt,
          ~limit=pageSize,
          ~offset,
          (),
        ),
    );
  result |> Sub.map(_, x => x##transactions->Belt_Array.map(toExternal));
};

let count = () => {
  let (result, _) = ApolloHooks.useSubscription(TxCountConfig.definition);
  result
  |> Sub.map(_, x => x##transactions_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};

let countBySender = sender => {
  let (result, _) =
    ApolloHooks.useSubscription(
      TxCountBySenderConfig.definition,
      ~variables=TxCountBySenderConfig.makeVariables(~sender=sender |> Address.toBech32, ()),
    );
  result
  |> Sub.map(_, a => {
       switch (a##accounts_by_pk) {
       | Some(account) =>
         account##account_transactions_aggregate##aggregate
         |> Belt_Option.getExn
         |> (y => y##count)
       | None => 0
       }
     });
};
