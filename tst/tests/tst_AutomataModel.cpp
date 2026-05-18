#include <catch2/catch.hpp>
#include "lib/RegexAST.hpp"
#include "lib/RegexDriver.hpp"
#include "lib/AutomataModel.hpp"

TEST_CASE("AutomataModel::AddAutomata, AutomataModel::RemoveAutomata, AutomataModel::RemoveAllAutomata, AutomataModel::GetAutomata, AutomataModel::GetAllAutomata")
{
    SECTION("01: AddAutomata - Basic case")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata = FiniteAutomata("a").Minimization();
        unsigned id = model->AddAutomata(automata);
        REQUIRE(id == 0);
    }

    SECTION("02: GetAutomata - Basic case")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata = FiniteAutomata("a").Minimization();
        model->AddAutomata(automata);
        std::optional<FiniteAutomata> automataFromModelOpt = model->GetAutomata(0);
        FiniteAutomata automataFromModel = *automataFromModelOpt;
        REQUIRE(automataFromModelOpt != std::nullopt);
        REQUIRE(automataFromModel.BuildRegex() == automata.BuildRegex());
    }

    SECTION("03: RemoveAutomata - Basic case")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata = FiniteAutomata("a").Minimization();
        model->AddAutomata(automata);
        std::optional<FiniteAutomata> removedAutomataOpt = model->RemoveAutomata(0);
        FiniteAutomata removedAutomata = *removedAutomataOpt;
        REQUIRE(removedAutomataOpt != std::nullopt);
        REQUIRE(removedAutomata.BuildRegex() == automata.BuildRegex());
    }

    SECTION("04: GetAllAutomata - Basic case")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata = FiniteAutomata("a").Minimization();
        model->AddAutomata(automata);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(automatas.size() == 1);
        REQUIRE(automatas[0].BuildRegex() == model->GetAutomata(0)->BuildRegex());
    }

    SECTION("05: RemoveAllAutomata - Basic case")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata = FiniteAutomata("a").Minimization();
        model->AddAutomata(automata);
        model->RemoveAllAutomata();
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(automatas.size() == 0);
    }

    SECTION("06: AddAutomata - Add automata after last automata is removed")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        FiniteAutomata automata2 = FiniteAutomata("c").Minimization();
        FiniteAutomata automata3 = FiniteAutomata("d").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        model->AddAutomata(automata2);
        model->RemoveAutomata(2);
        unsigned id = model->AddAutomata(automata3);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(id == 2);
        REQUIRE(automatas.size() == 3);
    }

    SECTION("07: AddAutomata - Add automata after middle automata is removed")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        FiniteAutomata automata2 = FiniteAutomata("c").Minimization();
        FiniteAutomata automata3 = FiniteAutomata("d").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        model->AddAutomata(automata2);
        model->RemoveAutomata(1);
        unsigned id = model->AddAutomata(automata3);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(id == 1);
        REQUIRE(automatas.size() == 3);
    }

    SECTION("08: AddAutomata - Add automata after 2 middle automatas are removed")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        FiniteAutomata automata2 = FiniteAutomata("c").Minimization();
        FiniteAutomata automata3 = FiniteAutomata("d").Minimization();
        FiniteAutomata automata4 = FiniteAutomata("e").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        model->AddAutomata(automata2);
        model->AddAutomata(automata3);
        model->RemoveAutomata(2);
        model->RemoveAutomata(1);
        unsigned id = model->AddAutomata(automata4);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(id == 2);
        REQUIRE(automatas.size() == 3);
    }

    SECTION("09: AddAutomata - Add recently removed automata")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        model->AddAutomata(automata0);
        std::optional<FiniteAutomata> removedAutomataOpt = model->RemoveAutomata(0);
        FiniteAutomata removedAutomata = *removedAutomataOpt;
        unsigned id = model->AddAutomata(removedAutomata);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(id == 0);
        REQUIRE(automatas.size() == 1);
    }

    SECTION("10: AddAutomata - Add automata after removing all automatas")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        model->RemoveAllAutomata();
        unsigned id = model->AddAutomata(automata0);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(id == 0);
        REQUIRE(automatas.size() == 1);
    }

    SECTION("11: RemoveAutomata - Remove non-existent automata")
    {
        AutomataModel* model = new AutomataModel();
        std::optional<FiniteAutomata> removedAutomataOpt = model->RemoveAutomata(0);
        REQUIRE(removedAutomataOpt == std::nullopt);
    }

    SECTION("12: RemoveAutomata - Remove out-of-range automata")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        std::optional<FiniteAutomata> removedAutomataOpt = model->RemoveAutomata(2);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(removedAutomataOpt == std::nullopt);
        REQUIRE(automatas.size() == 2);
    }

    SECTION("13: RemoveAutomata - Remove same automata twice")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        std::optional<FiniteAutomata> removedAutomataOpt0 = model->RemoveAutomata(1);
        std::optional<FiniteAutomata> removedAutomataOpt1 = model->RemoveAutomata(1);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(removedAutomataOpt0 != std::nullopt);
        REQUIRE(removedAutomataOpt1 == std::nullopt);
        REQUIRE(automatas.size() == 1);
    }

    SECTION("14: GetAutomata - Get non-existent automata")
    {
        AutomataModel* model = new AutomataModel();
        std::optional<FiniteAutomata> automataFromModelOpt = model->GetAutomata(0);
        REQUIRE(automataFromModelOpt == std::nullopt);
    }

    SECTION("15: GetAutomata - Get out-of-range automata")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        std::optional<FiniteAutomata> automataFromModelOpt = model->GetAutomata(2);
        REQUIRE(automataFromModelOpt == std::nullopt);
    }

    SECTION("16: GetAutomata - Get recently removed automata")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        std::optional<FiniteAutomata> removedAutomataOpt = model->RemoveAutomata(1);
        std::optional<FiniteAutomata> automataFromModelOpt = model->GetAutomata(1);
        REQUIRE(removedAutomataOpt != std::nullopt);
        REQUIRE(automataFromModelOpt == std::nullopt);
    }

    SECTION("17: GetAutomata - Get automata after removing all automatas")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata = FiniteAutomata("a").Minimization();
        model->AddAutomata(automata);
        model->RemoveAllAutomata();
        REQUIRE(model->GetAutomata(0) == std::nullopt);
    }

    SECTION("18: GetAllAutomata - Get all automata from empty model")
    {
        AutomataModel* model = new AutomataModel();
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(automatas.size() == 0);
    }

    SECTION("19: GetAllAutomata - Get all automata after removing single automata")
    {
        AutomataModel* model = new AutomataModel();
        FiniteAutomata automata0 = FiniteAutomata("a").Minimization();
        FiniteAutomata automata1 = FiniteAutomata("b").Minimization();
        model->AddAutomata(automata0);
        model->AddAutomata(automata1);
        std::optional<FiniteAutomata> removedAutomataOpt = model->RemoveAutomata(0);
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(automatas[1].BuildRegex() == automata1.BuildRegex());
        REQUIRE(automatas.size()==1);
    }

    SECTION("20: RemoveAllAutomata - Remove all automata from empty model")
    {
        AutomataModel* model = new AutomataModel();
        model->RemoveAllAutomata();
        std::map<unsigned,FiniteAutomata> automatas = model->GetAllAutomata();
        REQUIRE(automatas.size() == 0);
    }
}
