/*
 * Copyright (c) 2011-2014, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <list>
#include "XmlSink.h"
#include "XmlSource.h"

#include "PathNavigator.h"

class CXmlElementSerializingContext;
class CErrorContext;

class CElement : public IXmlSink, public IXmlSource
{
    friend class CAutoLog;
public:
    CElement(const std::string& strName = "");
    virtual ~CElement();

    // Logging
    void log_info(const std::string& strMessage, ...) const;
    void log_warning(const std::string& strMessage, ...) const;
    void log_table(bool bIsWarning, const std::list<std::string> lstrMessage) const;

    // Description
    void setDescription(const std::string& strDescription);
    const std::string& getDescription() const;

    // Name / Path
    const std::string& getName() const;
    void setName(const std::string& strName);
    bool rename(const std::string& strName, std::string& strError);
    std::string getPath() const;
    std::string getQualifiedPath() const;

    // Creation / build
    virtual bool init(std::string& strError);
    virtual void clean();

    // Children management
    void addChild(CElement* pChild);
    bool removeChild(CElement* pChild);
    void listChildren(std::string& strChildList) const;
    std::string listQualifiedPaths(bool bDive, uint32_t uiLevel = 0) const;
    void listChildrenPaths(std::string& strChildPathList) const;

    // Hierarchy query
    uint32_t getNbChildren() const;
    CElement* findChildOfKind(const std::string& strKind);
    const CElement* findChildOfKind(const std::string& strKind) const;
    const CElement* getParent() const;
    const CElement* getChild(uint32_t uiIndex) const;
    CElement* getChild(uint32_t uiIndex);
    const CElement* findChild(const std::string& strName) const;
    CElement* findChild(const std::string& strName);
    const CElement* findDescendant(CPathNavigator& pathNavigator) const;
    CElement* findDescendant(CPathNavigator& pathNavigator);
    bool isDescendantOf(const CElement* pCandidateAscendant) const;

    // From IXmlSink
    virtual bool fromXml(const CXmlElement& xmlElement, CXmlSerializingContext& serializingContext);

    // From IXmlSource
    virtual void toXml(CXmlElement& xmlElement, CXmlSerializingContext& serializingContext) const;

    /**
     * Serialize the children to XML
     *
     * This method is virtual, to be derived in case a special treatment is
     * needed before doing so.
     *
     * @param[in,out] xmlElement the XML Element below which the children must
     *                be serialized (which may or may not be the CElement
     *                object upon which this method is called)
     * @param[in,out] serializingContext information about the serialization
     */
    virtual void childrenToXml(CXmlElement& xmlElement,
                               CXmlSerializingContext& serializingContext) const;

    // Content structure dump
    void dumpContent(std::string& strContent, CErrorContext& errorContext, const uint32_t uiDepth = 0) const;

    // Element properties
    virtual void showProperties(std::string& strResult) const;

    // Conversion utilities
    static std::string toString(uint32_t uiValue);
    static std::string toString(uint64_t uiValue);
    static std::string toString(int32_t iValue);
    static std::string toString(double dValue);

    // Checksum for integrity checks
    uint8_t computeStructureChecksum() const;

    // Class kind
    virtual std::string getKind() const = 0;
protected:
    // Content dumping
    virtual void logValue(std::string& strValue, CErrorContext& errorContext) const;
    // Utility to underline
    static void appendTitle(std::string& strTo, const std::string& strTitle);

    // Hierarchy
    CElement* getLastChild();
    CElement* getParent();
    CElement* findAscendantOfKind(const std::string& strKind);
    CElement* getRoot();
    const CElement* getRoot() const;

    /**
     * Creates a child CElement from a child XML Element
     *
     * @param[in] childElement the XML element to create CElement from
     * @param[in] elementSerializingContext the serializing context
     *
     * @return child a pointer on the CElement object that has been added to the tree
     */
    CElement* createChild(const CXmlElement& childElement,
                          CXmlSerializingContext& elementSerializingContext);

private:
    // Logging (done by root)
    virtual void doLog(bool bIsWarning, const std::string& strLog) const;
    virtual void nestLog() const;
    virtual void unnestLog() const;
    // Returns Name or Kind if no Name
    std::string getPathName() const;
    // Returns true if children dynamic creation is to be dealt with
    virtual bool childrenAreDynamic() const;
    // House keeping
    void removeChildren();
    // For logging
    uint32_t getDepth() const;
    // Fill XmlElement during XML composing
    void setXmlNameAttribute(CXmlElement& xmlElement) const;

    // Name
    std::string _strName;

    // Description
    std::string _strDescription;

    // Child iterators
    typedef std::vector<CElement*>::iterator ChildArrayIterator;
    typedef std::vector<CElement*>::reverse_iterator ChildArrayReverseIterator;
    // Children
    std::vector<CElement*> _childArray;
    // Parent
    CElement* _pParent;
};
