#ifndef ORGANISM_H
#define ORGANISM_H

#include <optional>
#include <cstdint>
#include <tuple>

constexpr bool is_movable(bool eats_m, bool eats_p);

constexpr bool same_food_preferences(bool sp1_eats_m, bool sp1_eats_p,
                                     bool sp2_eats_m, bool sp2_eats_p);

template <typename species_t, bool can_eat_meat, bool can_eat_plants>
requires std::equality_comparable<species_t>
class Organism;

template <typename species_t>
using Carnivore = Organism<species_t, true, false>;

template <typename species_t>
using Omnivore = Organism<species_t, true, true>;

template <typename species_t>
using Herbivore = Organism<species_t, false, true>; 

template <typename species_t>
using Plant = Organism<species_t, false, false>;

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
requires(is_movable(sp1_eats_m, sp1_eats_p) || is_movable(sp2_eats_m, sp2_eats_p))
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
          Organism<species_t, sp2_eats_m, sp2_eats_p>,
          std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2);

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args... args);

///-----------------------------------------------------------------------------------------------

template <typename species_t, bool can_eat_meat, bool can_eat_plants>
requires std::equality_comparable<species_t>
class Organism {
public:
    constexpr Organism(species_t const &species, uint64_t vitality) :
            species(species), vitality(vitality) { }

    constexpr Organism<species_t, can_eat_meat, can_eat_plants> dead() {
        return {Organism<species_t, can_eat_meat, can_eat_plants>(species, 0)};
    }

    constexpr Organism<species_t, can_eat_meat, can_eat_plants> fed_with(uint64_t food) {
        return {Organism<species_t, can_eat_meat, can_eat_plants>(species, vitality + food)};
    }

    constexpr bool is_animal() { return can_eat_meat || can_eat_plants; }

    constexpr bool is_plant() { return !is_animal(); }

    constexpr uint64_t get_vitality() const { return vitality; }

    constexpr species_t const &get_species() const {
        return species;
    }

    constexpr bool is_dead() const { return vitality == 0; }

private: 
    species_t const &species;
    uint64_t vitality;
};

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
        Organism<species_t, sp2_eats_m, sp2_eats_p>,
        std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
no_effect(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {
    return std::make_tuple(Organism(organism1), Organism(organism2), std::nullopt);
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
// Możliwe jest spotkanie jedynie organizmów, których typ gatunku (species_t) jest taki sam
// Nie jest możliwe spotkanie dwóch roślin,
requires(is_movable(sp1_eats_m, sp1_eats_p) || is_movable(sp2_eats_m, sp2_eats_p))
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
        Organism<species_t, sp2_eats_m, sp2_eats_p>,
        std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {

    // Spotkanie, w którym jedna ze stron jest martwa, nie powoduje skutków.
    if ((organism1.get_vitality() == 0) || (organism2.get_vitality() == 0)) {
        return no_effect(organism1, organism2);
    }

    // Spotkanie dwóch zwierząt tego samego gatunku prowadzi do godów.
    if ((organism1.get_species() == organism2.get_species()) &&
    same_food_preferences(sp1_eats_m, sp1_eats_p, sp2_eats_m, sp2_eats_p)) {
        auto species = organism1.get_species();
        int vitality1 = organism1.get_vitality();
        int vitality2 = organism2.get_vitality();

        return std::make_tuple(Organism(organism1), Organism(organism2),
                Organism<species_t, sp1_eats_m, sp1_eats_p>(species, vitality1 + vitality2 / 2));
    }

    // Dokładnie jeden jest rośliną
    if (organism1.is_plant() || organism2.is_plant()) {
        if (sp1_eats_p) {
            return std::make_tuple
            (organism1.fed_with(organism2.get_vitality()), organism2.dead(), std::nullopt);
        }
        else if (sp2_eats_p) {
            return std::make_tuple
            (organism1.dead(), organism2.fed_with(organism1.get_vitality()), std::nullopt);
        }
        else {
            return no_effect(organism1, organism2);
        }
    }

    // Od tego momentu oba organizmy to zwierzęta.

    // Oba mięsożerne, walka.
    if (sp1_eats_m && sp2_eats_m) {
        if (organism1.get_vitality() > organism2.get_vitality()) {
            return std::make_tuple
            (organism1.fed_with(organism2.get_vitality() / 2), organism2.dead(), std::nullopt);
        }
        else if (organism1.get_vitality() < organism2.get_vitality()) {
            return std::make_tuple
            (organism1.dead(), organism2.fed_with(organism1.get_vitality() / 2), std::nullopt);
        }
        else {
            return std::make_tuple(organism1.dead(), organism2.dead(), std::nullopt);
        }
    }

    // Dokładnie jeden mięsożerny, zjada lub nie drugiego.
    if (sp1_eats_m) {
        if (organism1.get_vitality() <= organism2.get_vitality()) {
            return no_effect(organism1, organism2);
        }
        else {
            return std::make_tuple
            (organism1.fed_with(organism2.get_vitality() / 2), organism2.dead(), std::nullopt);
        }
    }

    if (sp2_eats_m) {
        if (organism1.get_vitality() >= organism2.get_vitality()) {
            return no_effect(organism1, organism2);
        }
        else {
            return std::make_tuple
            (organism1.dead(), organism2.fed_with(organism1.get_vitality() / 2), std::nullopt);
        }
    }

    // Oba nie jedzą mięsa, brak efektu.
    return no_effect(organism1, organism2);
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename T, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series_helper(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                        T organism2, Args... args) {
    if constexpr (sizeof...(args) > 0) {
        return encounter_series_helper(std::get<0> (encounter(organism1, organism2)), args...);
    }

    return std::get<0>(encounter(organism1, organism2));
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return organism1;
    }

    return encounter_series_helper(organism1, args...);
}

constexpr bool is_movable(bool eats_m, bool eats_p) { return eats_m || eats_p; }

constexpr bool same_food_preferences(bool sp1_eats_m, bool sp1_eats_p,
                                     bool sp2_eats_m, bool sp2_eats_p) {
    return ((sp1_eats_m == sp2_eats_m) && (sp1_eats_p == sp2_eats_p));
}

#endif // ORGANISM_H