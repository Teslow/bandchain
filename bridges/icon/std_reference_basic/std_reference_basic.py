from iconservice import *
from .pyobi import *

TAG = "StdReferenceBasic"


class StdReferenceBasic(IconScoreBase):

    # rate: USD-rate, multiplied by 1e9.
    # last_update: UNIX epoch when data is last updated in milisec.
    REF_DATA = PyObi("{rate:u64,last_update:u64}")

    REFERENCE_DATA = PyObi("{rate:u64,last_update_base:u64,last_update_quote:u64}")
    RELAY_DATA = PyObi("[{symbol:string,rate:u64,resolve_time:u64}]")
    PAIRS = PyObi("[{base:string,quote:string}]")

    ONE = 1_000_000_000
    MICRO_SEC = 1_000_000

    @eventlog
    def RefDataUpdate(self, _symbol: str, _rate: int, _lastUpdate: int):
        pass

    def __init__(self, db: IconScoreDatabase) -> None:
        super().__init__(db)
        # Mapping from symbol to obi encoded ref data.
        self.refs = DictDB("refs", db, value_type=bytes)

    def on_install(self) -> None:
        super().on_install()

    def on_update(self) -> None:
        super().on_update()

    def _get_ref_data(self, _symbol: str) -> dict:
        if _symbol == "USD":
            return {"rate": self.ONE, "last_update": self.block.timestamp}

        if self.refs[_symbol] == None:
            self.revert("REF_DATA_NOT_AVAILABLE")

        return self.REF_DATA.decode(self.refs[_symbol])

    def _get_reference_data(self, _base: str, _quote: str) -> dict:
        ref_base = self._get_ref_data(_base)
        ref_quote = self._get_ref_data(_quote)
        return {
            "rate": (ref_base["rate"] * self.ONE * self.ONE) // ref_quote["rate"],
            "last_update_base": ref_base["last_update"],
            "last_update_quote": ref_quote["last_update"],
        }

    @external(readonly=True)
    def get_ref_data(self, _symbol: str) -> dict:
        return self._get_ref_data(_symbol)

    @external(readonly=True)
    def get_reference_data(self, _base: str, _quote: str) -> dict:
        return self._get_reference_data(_base, _quote)

    @external(readonly=True)
    def get_reference_data_bulk(self, _encoded_pairs: bytes) -> list:
        return [
            self._get_reference_data(pair["base"], pair["quote"])
            for pair in self.PAIRS.decode(_encoded_pairs)
        ]

    def _set_refs(self, _symbol: str, _rate: int, _last_update: int) -> None:
        self.refs[_symbol] = self.REF_DATA.encode(
            {"rate": _rate, "last_update": _last_update * self.MICRO_SEC}
        )

    @external
    def relay(self, _encoded_data_list: bytes) -> None:
        if self.msg.sender != self.owner:
            self.revert("NOT_AUTHORIZED")

        for data in self.RELAY_DATA.decode(_encoded_data_list):
            # set rate and last_update for the symbol
            self._set_refs(data["symbol"], data["rate"], data["resolve_time"])

            # emit event log
            self.RefDataUpdate(data["symbol"], data["rate"], data["resolve_time"])

