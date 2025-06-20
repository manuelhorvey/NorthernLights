# TheNorthernLights

**Project Type:** C++ Modular Trading System
**Target OS:** Linux (Fedora optimized)
**Core Focus:** Order block detection, market structure analysis, regime switching, and execution infrastructure for systematic trading.

---

## 🚀 Project Overview

**TheNorthernLights** is a modular, high-performance C++ trading engine designed for research and real-time execution of algorithmic strategies. It supports structured data ingestion, rule-based and ML-enhanced signal generation, order execution logic, and continuous optimization.

The system is broken into clean submodules for ingestion, strategy modeling, trade execution, risk/position control, and optimization. Python integration is included via virtualenv for tooling, data handling, and backtesting utilities.

---

## 🧱 Directory Structure

```
TheNorthernLights/
├── include/              # Public headers (interfaces)
├── src/                  # Core logic implementation
├── config/               # Strategy, risk, and regime config files
├── data/                 # Market data and features
├── tests/                # Unit and integration tests
├── benchmarks/           # Performance profiling
├── scripts/              # Setup, deployment, backtesting scripts
├── third_party/          # Vendored external libs
├── docs/                 # Design docs and theoretical notes
├── ui/                   # Optional frontend (web/CLI)
├── build/                # Build artifacts (ignored)
├── logs/                 # Runtime logs
├── CMakeLists.txt        # Project build configuration
├── bootstrap.sh          # Fedora-optimized bootstrap script
├── README.md             # Quick start and overview
├── VERSION               # Versioning
└── .gitignore / .clang-format
```

---

## ⚙️ Modules Breakdown

### 1. `data/`

* **market\_data.cpp/.hpp** – Ingests tick, OHLCV, and order book data.
* **alt\_data.cpp/.hpp** – Handles sentiment, news, and macroeconomic feeds.
* **features.cpp/.hpp** – Transforms raw data into technical or statistical features.

### 2. `strategy/`

* **rule\_engine** – Rule-based detection of order blocks, BOS/CHoCH, liquidity sweeps.
* **ml\_engine** – Optional ML models for classification or regression signals.
* **regime\_switcher** – Uses filters or HMMs to adapt strategies to volatility regimes.
* **signal\_scorer** – Assigns confidence to signals using ranking logic.

### 3. `execution/`

* **order\_router** – Handles broker communication and route logic.
* **position\_sizer** – Determines trade size based on volatility, capital, and risk.
* **trade\_executor** – Executes trades and manages fills/slippage.

### 4. `optimization/`

* **tuner** – Genetic or Bayesian parameter tuning.
* **walkforward** – Rolling window backtest with strategy adaptation.
* **feedback\_loop** – Trade journaling and live strategy learning.

### 5. `infra/`

* **scheduler** – Manages tick/candle event loops.
* **rest\_server** – Optional HTTP API layer.
* **orchestrator** – Wires together modules into unified workflows.

### 6. `utils/`

* **logger** – spdlog wrapper for consistent logging.
* **math** – Statistical tools, indicators, helper math functions.
* **config** – Loads and parses TOML, JSON, and INI configs.

---

## 🛠️ Build and Dev Environment

### Fedora-based Bootstrap:

Run the following to install system packages, set up Python, and initialize CMake:

```bash
./scripts/bootstrap.sh
```

### Manual Steps:

```bash
sudo dnf install -y cmake gcc-c++ git python3 python3-virtualenv fmt-devel spdlog-devel catch2-devel
python3 -m venv .venv && source .venv/bin/activate
mkdir build && cd build && cmake .. && make -j
```

---

## 🧪 Testing

* Catch2 is used for unit and integration testing.
* All test files reside in `/tests/`
* Run tests:

```bash
./build/run_tests
```

---

## 🔬 Benchmarking

Benchmark modules in `/benchmarks/` help measure performance of core components:

* Math ops
* Execution latency
* Signal processing

Use Google Benchmark or simple time macros (planned: JIT profiling).

---

## 📊 Config and Strategy Definitions

Configs live under `config/` and `config/templates/`:

* `strategy.cfg` – Core strategy parameters
* `risk.cfg` – Stop loss, TP, position risk rules
* `routes.json` – Broker routing configuration
* `regime_profiles.toml` – Mapping vol regimes to behavior

Template configs enable quick switching between strategy archetypes.

---

## 🧠 Strategy Theory and Documentation

Documentation lives in `/docs/`:

* `architecture_overview.md` – End-to-end flow of the system
* `performance_metrics.md` – How strategies are evaluated
* `regime_theory.pdf` – Notes on volatility regime modeling
* `signal_flowchart.svg` – Visual layout of signal processing pipeline

---

## 💻 UI (Optional)

Simple web dashboard components:

* `dashboard.html` – Static visualization
* `signal_heatmap.js` – Renders signal intensity
* `ws_client.js` – Real-time updates via WebSocket (future: plug into broker stream)

---

## 🧩 Extending the System

* Add new ML models via `strategy/ml_engine.*`
* Add new indicators in `features.cpp`
* Create walk-forward experiment profiles in `backtest_runner.py`
* Plug in exchange APIs in `order_router.cpp`

---

## 🧭 Vision & Goal

This project is designed to be:

* 💡 **Readable**: Modular components with strong interfaces
* ⚡ **High-Performance**: C++ core with Python scripting tools
* 🤖 **Smart**: ML-ready, regime-aware, data-rich
* 📈 **Alpha-oriented**: Focused on real edge extraction, not toy strategies

> Start simple. Scale modularly. Let the data guide evolution.

---

## ✅ Todos & Next Steps

* [ ] Add support for JSON-RPC & WebSocket execution API
* [ ] Integrate SQLite or DuckDB for local data persistence
* [ ] Enable backtest parameter sweep via CLI flags
* [ ] Plug in TensorRT ONNX inference for low-latency ML
* [ ] Unit test coverage: 85% target

---

## 📜 License

Currently private. May be licensed under Apache 2.0 or MIT upon stabilization.
