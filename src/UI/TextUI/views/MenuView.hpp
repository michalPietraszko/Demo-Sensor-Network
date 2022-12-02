#pragma once
#include <MenuViewFormater.hpp>
#include <Renderer.hpp>
#include <UIStatusCell.hpp>
#include <UITableView.hpp>
#include <UITextField.hpp>
#include <string>
#include <vector>

template <typename Descriptor = std::string>
class MenuView {
public:
    using MappedMenu = UITableView<Descriptor, UIStatusCell>;
    using MappedMenuEntry = typename MappedMenu::MappedCell;

    MenuView(UITextField headline, MappedMenu menu) : headline{std::move(headline)}, menu{std::move(menu)} {}

    static MappedMenuEntry createMenuEntry(Descriptor descriptor, UIStatusCell content = {}) {
        return {descriptor, content};
    }

    void display(Renderer& renderer) {
        renderer.submit(headline, MenuViewFormater::headline);
        for (auto& entry : menu) {
            renderer.submit(entry.second.description, MenuViewFormater::cellName);
            renderer.submit(entry.second.status, MenuViewFormater::cellStatus);
        }
    }

public:
    UITextField headline;
    MappedMenu menu;
};
