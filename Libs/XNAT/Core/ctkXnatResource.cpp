/*=============================================================================

  Library: XNAT/Core

  Copyright (c) University College London,
    Centre for Medical Image Computing

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include "ctkXnatResource.h"

#include "ctkXnatConstants.h"
#include "ctkXnatObjectPrivate.h"
#include "ctkXnatSession.h"

//----------------------------------------------------------------------------
class ctkXnatResourcePrivate : public ctkXnatObjectPrivate
{
public:

  ctkXnatResourcePrivate()
  : ctkXnatObjectPrivate()
  {
  }
};


//----------------------------------------------------------------------------
ctkXnatResource::ctkXnatResource(ctkXnatObject* parent, const QString& schemaType)
: ctkXnatObject(*new ctkXnatResourcePrivate(), parent, schemaType)
{
}

//----------------------------------------------------------------------------
ctkXnatResource::~ctkXnatResource()
{
}

//----------------------------------------------------------------------------
QString ctkXnatResource::resourceUri() const
{
  return QString("%1/resources/%2").arg(parent()->resourceUri(), this->id());
}

//----------------------------------------------------------------------------
QString ctkXnatResource::id() const
{
  return property(ctkXnatObjectFields::ABSTRACT_RESOURCE_ID);
}

//----------------------------------------------------------------------------
void ctkXnatResource::setId(const QString &id)
{
  setProperty(ctkXnatObjectFields::ABSTRACT_RESOURCE_ID, id);
}

//----------------------------------------------------------------------------
QString ctkXnatResource::name() const
{
  return this->label();
}

//----------------------------------------------------------------------------
void ctkXnatResource::setName(const QString &name)
{
  this->setLabel(name);
}

//----------------------------------------------------------------------------
QString ctkXnatResource::label() const
{
  return property(ctkXnatObjectFields::LABEL);
}

//----------------------------------------------------------------------------
void ctkXnatResource::setLabel(const QString &label)
{
  setProperty(ctkXnatObjectFields::LABEL, label);
}

//----------------------------------------------------------------------------
void ctkXnatResource::reset()
{
  ctkXnatObject::reset();
}

//----------------------------------------------------------------------------
void ctkXnatResource::fetchImpl()
{
  QString resourceFilesUri = this->resourceUri() + "/files";
  ctkXnatSession* const session = this->session();
  QUuid queryId = session->httpGet(resourceFilesUri);

  QList<ctkXnatObject*> files = session->httpResults(queryId,
                                                     ctkXnatDefaultSchemaTypes::XSI_FILE);

  foreach (ctkXnatObject* file, files)
  {
    QString label = file->name();
    if (label.isEmpty())
    {
      label = "NO NAME";
    }
    file->setName(label);
    this->add(file);
  }
}

//----------------------------------------------------------------------------
void ctkXnatResource::downloadImpl(const QString& filename)
{
  QString query = this->resourceUri() + "/files";
  ctkXnatSession::UrlParameters parameters;
  parameters["format"] = "zip";
  this->session()->download(filename, query, parameters);
}
