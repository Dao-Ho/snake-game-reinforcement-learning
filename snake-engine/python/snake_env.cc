#include "snake/board.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(snake_engine_py, m) {
  py::enum_<SnakeEngine::Move>(m, "Move")
      .value("kUp", SnakeEngine::Move::kUp)
      .value("kDown", SnakeEngine::Move::kDown)
      .value("kLeft", SnakeEngine::Move::kLeft)
      .value("kRight", SnakeEngine::Move::kRight);

  py::enum_<SnakeEngine::Cell>(m, "Cell")
      .value("kEmpty", SnakeEngine::Cell::kEmpty)
      .value("kSnakeBody", SnakeEngine::Cell::kSnakeBody)
      .value("kSnakeHead", SnakeEngine::Cell::kSnakeHead)
      .value("kApple", SnakeEngine::Cell::kApple);

  py::class_<SnakeEngine::Board>(m, "Board")
      .def(py::init<int, int>())
      .def("apply_move",
           [](SnakeEngine::Board &board, SnakeEngine::Move move) {
             auto result = board.ApplyMove(move);
             if (!result.ok()) {
               throw py::value_error(std::string(result.status().message()));
             }
             return result.value();
           })
      .def("is_game_over", &SnakeEngine::Board::IsGameOver)
      .def("get_grid", &SnakeEngine::Board::GetGrid)
      .def("get_score", &SnakeEngine::Board::GetScore)
      .def("get_width", &SnakeEngine::Board::GetWidth)
      .def("get_height", &SnakeEngine::Board::GetHeight);
}