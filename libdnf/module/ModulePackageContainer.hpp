/*
 * Copyright (C) 2018 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef LIBDNF_MODULEPACKAGECONTAINER_HPP
#define LIBDNF_MODULEPACKAGECONTAINER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <set>

#include "ModulePackage.hpp"

struct ModulePackageContainer
{
public:
    struct Exception : public std::runtime_error
    {
        explicit Exception(const std::string &what) : runtime_error(what) {}
    };

    struct NoModuleException : public Exception
    {
        explicit NoModuleException(const std::string &moduleName) : Exception("No such module: " + moduleName) {}
    };

    struct NoStreamException : public Exception
    {
        explicit NoStreamException(const std::string &moduleStream) : Exception("No such stream: " + moduleStream) {}
    };

    struct EnabledStreamException : public Exception
    {
        explicit EnabledStreamException(const std::string &moduleName) : Exception("No enabled stream for module: " + moduleName) {}
    };

    explicit ModulePackageContainer(const char * arch);
    ~ModulePackageContainer();

    void add(HyRepo repo, const std::string &fileContent);
    void createConflictsBetweenStreams();
    std::shared_ptr<ModulePackage> getModulePackage(Id id);
    std::vector<std::shared_ptr<ModulePackage>> getModulePackages();

    std::vector<std::shared_ptr<ModulePackage>> requiresModuleEnablement(const libdnf::PackageSet & packages);
    void enable(const std::string &name, const std::string &stream);

    void resolveActiveModulePackages(const std::map<std::string, std::string> &defaultStreams);
    bool isModuleActive(Id id);
    Pool * getPool();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};


#endif //LIBDNF_MODULEPACKAGECONTAINER_HPP