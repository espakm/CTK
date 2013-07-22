/*=============================================================================

  Plugin: org.commontk.xnat

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

#include "ctkXnatObject.h"
#include "ctkXnatObjectPrivate.h"

#include "ctkXnatServer.h"
#include <QVariant>
#include <QDebug>

ctkXnatObject::~ctkXnatObject()
{
}

QString ctkXnatObject::getId() const
{
  return getProperty("ID");
}

QString ctkXnatObject::getName() const
{
  return getProperty("name");
}

QString ctkXnatObject::getDescription() const
{
  return getProperty("description");
}

QString ctkXnatObject::getProperty(const QString& name) const
{
  Q_D(const ctkXnatObject);
  ctkXnatObjectPrivate::PropertyMapConstInterator iter = d->properties.find(name);
  if (iter != d->properties.end())
  {
    return iter.value();
  }
  return QString::null;
}

void ctkXnatObject::setProperty(const QString& name, const QVariant& value)
{
  Q_D(ctkXnatObject);
  d->properties.insert(name, value.toString());
}

ctkXnatObject::Pointer ctkXnatObject::getParent() const
{
  Q_D(const ctkXnatObject);
  return d->parent;
}

QList<ctkXnatObject::Pointer> ctkXnatObject::getChildren() const
{
  Q_D(const ctkXnatObject);
  return d->children;
}

void ctkXnatObject::addChild(ctkXnatObject::Pointer& child)
{
  Q_D(ctkXnatObject);
  d->children.push_back(child);
  child->d_func()->parent = d->selfPtr;
}

void ctkXnatObject::reset()
{
  Q_D(ctkXnatObject);
  //d->properties.clear();
  d->children.clear();
}

void ctkXnatObject::fetch()
{
  this->reset();
  this->fetchImpl();
}

void ctkXnatObject::download(const QString& /*zipFilename*/)
{
  // do nothing
}

void ctkXnatObject::upload(const QString& /*zipFilename*/)
{
  // do nothing
}

void ctkXnatObject::add(const QString& /*name*/)
{
  // do nothing
}

void ctkXnatObject::remove()
{
  // do nothing
}

bool ctkXnatObject::isModifiable() const
{
  return false;
}

ctkXnatObject::ctkXnatObject()
: d_ptr(new ctkXnatObjectPrivate())
{
}

ctkXnatObject::ctkXnatObject(ctkXnatObjectPrivate* d)
: d_ptr(d)
{
}

ctkXnatConnection* ctkXnatObject::getConnection() const
{
  const ctkXnatObject* xnatObject = this;
  const ctkXnatServer* server;
  do {
    xnatObject = xnatObject->getParent().data();
    server = dynamic_cast<const ctkXnatServer*>(xnatObject);
  }
  while (xnatObject && !server);

  return server ? xnatObject->getConnection() : 0;
}

void ctkXnatObject::setId(const QString& id)
{
  setProperty("ID", id);
}

bool ctkXnatObject::isDeletable() const
{
  return false;
}
